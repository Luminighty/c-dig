#include "client/client.h"
#include "client/args.h"
#include "client/client_app.h"
#include "components.h"
#include "config.h"
#include "darray.h"
#include "entity.h"
#include "entity_id.h"
#include "entity_prefabs.h"
#include "game.h"
#include "linalg.h"
#include "net_entity.h"
#include "netapi.h"
#include "netmessage.h"
#include "utils.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	NetClient *client;
	int ping_delay;
	NetworkEntities remote_entities;
	NetworkEntities owned_entities;
} GameNetClient;


static GameNetClient client = {0};

static inline void tcp_send_message(GameNetClient *client, NetPackedMessage *packet) {
	client_tcp_send(client->client, packet->buffer, packet->size);
}

void game_client_net_init() {
	client = (GameNetClient){0};
	printf("Trying to connect...\n");
	client.client = client_connect(cmd_args.server_ipdarr, cmd_args.port);
	printf("Connected...\n");
}


static inline void client_ping(GameNetClient *client) {
	if (client->ping_delay > 0) {
		client->ping_delay--;
		return;
	}
	client->ping_delay = PING_DELAY;
	NetMessageHeader header = {
		.kind = NET_MESSAGE_PING,
	};
	client_tcp_send(client->client, &header, sizeof(header));
}


static inline void sync_entity(GameNetClient *client, Entity entity) {
	NetworkEntity *net_entity = entity_get_networkentity(game.world, entity);
	// TODO: Temporarily disabled this, but should be re-enabled
	// if (!net_entity->dirty) return;

	Position *position = entity_get_position(game.world, entity);
	NetMessageEntityPositionUpdate body = {
		.net_id = net_entity->id,
		.position = *position,
	};
	NetPackedMessage packet = net_message_pack(NET_MESSAGE_ENTITY_POSITION_UPDATE, &body);
	tcp_send_message(client, &packet);
}

static void client_send_all_sync(GameNetClient *client) {
	for (size_t i = 0; i < client->owned_entities.count; i++) {
		sync_entity(client, client->owned_entities.items[i].entity);
	}
}

typedef enum {
	NET_READ_NONE,
	NET_READ_OK,
	NET_READ_ERR,
} NetReadResult;

static inline NetReadResult client_try_read(
	GameNetClient *client,
	NetMessageHeader *header, NetMessageBody *body
) {
	// Peek Header
	NetResult res = client_tcp_peek_into(client->client, header, sizeof(*header));
	if (res.status == NET_ERR)
		return NET_READ_ERR;
	if (res.status == NET_NONE || res.length < sizeof(*header))
		return NET_READ_NONE;

	if (header->length > sizeof(*body)) // Invalid header
		return NET_READ_ERR;
	
	// Peek Body
	ssize_t full_message_length = sizeof(*header) + header->length;
	ssize_t available = client_tcp_peek(client->client);
	if (available < 0) return NET_READ_ERR;
	if (available < full_message_length) return NET_READ_NONE;

	// Recv Header
	res = client_tcp_recv(client->client, header, sizeof(*header));
	if (res.status != NET_OK) {
		printf("[NET_READ_ERR] RecvHeader\n");
		return NET_READ_ERR;
	}

	// Recv Body
	if (header->length > 0) {
		res = client_tcp_recv(client->client, body, header->length);
		if (res.status != NET_OK) {
			printf("[NET_READ_ERR] Recv_Body\n");
			return NET_READ_ERR;
		}
	}
	return NET_READ_OK;
}


static inline void handle_position_update(GameNetClient *client, NetMessageHeader *header, void *body) {
	if (header->length != sizeof(NetMessageEntityPositionUpdate))
		return;
	NetMessageEntityPositionUpdate *update = body;
	Entity entity = network_entity_find(&client->remote_entities, update->net_id);
	// TODO: We will need to do more syncing,
	//       but this is just a quick get-it-working code
	if (entity_is_alive(game.world, entity)) {
		Position *position = entity_get_position(game.world, entity);

		position->x = update->position.x;
		position->y = update->position.y;
	} else {
		// TODO: The client should ask the server for what the heck this entity is, to which the server could reply with a spawn entity call
	}
}

static inline void handle_entity_spawn_prefab(GameNetClient *client, NetMessageHeader *header, void *body) {
	if (header->length != sizeof(NetMessageEntitySpawnPrefab))
		return;
	NetMessageEntitySpawnPrefab *spawn = body;
	printf("[EnittySpawnPrefab]\n");
	printf("   Owner: %d\n", spawn->owner);
	printf("   NetId: %d\n", spawn->net_id);

	Entity entity = entity_create_miner(game.world, spawn->props);

	entity_add_networkentity(game.world, entity, (NetworkEntity){
		.id = spawn->net_id, .owner = spawn->owner,
	});

	bool is_client_owned = spawn->owner == client_get_id(client->client);
	printf("  Owned?: %d\n", is_client_owned);
	
	NetworkEntities *container = is_client_owned
		? &client->owned_entities
		: &client->remote_entities;
	da_push(*container, ((NetworkEntityData){
		.id = spawn->net_id, .entity = entity,
	}));

	// NOTE: Disable network driven components
	if (is_client_owned) {
		return;
	}
	if (entity_has_component(game.world, entity, COMPONENT_RIGIDBODY)) {
		Rigidbody *rb = entity_get_rigidbody(game.world, entity);
		rb->static_body = true;
	}

}

static inline void handle_player_init(GameNetClient *client, NetMessageHeader *header, void *body) {
	if (header->length != sizeof(NetMessagePlayerInit))
		return;
	UNUSED(client);
	NetMessagePlayerInit *player_init = body;
	printf("[PlayerINIT]\n");
	printf("   NetId: %d\n", player_init->player_entity);
	client_app.player.entity = network_entity_find(&client->owned_entities, player_init->player_entity);
	if (!entity_is_alive(game.world, client_app.player.entity)) {
		printf("Got player INIT, but entity not found");
	}
}


static inline void client_update(GameNetClient *client) {
	static const int MAX_MESSAGES_PER_FRAME = 32;
	NetMessageHeader header = {0};
	NetMessageBody body = {0};
	int message_index = 0;
	while (message_index++ < MAX_MESSAGES_PER_FRAME) {
		NetReadResult result = client_try_read(client, &header, &body);
		if (result == NET_READ_NONE) return;
		// TODO: What should I do if I error out?
		if (result == NET_READ_ERR) return;

		switch (header.kind) {
		case NET_MESSAGE_PING: break; // Always updates ping
		case NET_MESSAGE_CLOSE: return; // TODO: Disconnected forcefully
		case NET_MESSAGE_ENTITY_SPAWN_PREFAB:
			handle_entity_spawn_prefab(client, &header, &body);
			break;
		case NET_MESSAGE_PLAYER_INIT:
			handle_player_init(client, &header, &body);
			break;
		case NET_MESSAGE_ENTITY_POSITION_UPDATE:
			handle_position_update(client, &header, &body);
			break;
		default: break;
		}
	}
}


void game_client_net_update() {
	client_update(&client);
	client_ping(&client);
	client_send_all_sync(&client);
}


void game_client_net_destroy() {
	client_disconnect(client.client);
	client.client = NULL;
}

