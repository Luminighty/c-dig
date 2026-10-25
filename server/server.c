#include "server/server.h"
#include "components.h"
#include "config.h"
#include "darray.h"
#include "display.h"
#include "entity_id.h"
#include "entity_prefabs.h"
#include "game.h"
#include "net_entity.h"
#include "netentity_id.h"
#include "random.h"
#include "server/args.h"
#include "netapi.h"
#include "netmessage.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_PING_DELAY (PING_DELAY * 3)

typedef struct {
	NetId id;
	int ping_timeout;
	Entity player;
} GameNetClient;

typedef DynamicArray(GameNetClient) GameClients;

typedef struct {
	NetServer *server;
	GameClients clients;
	NetworkEntities entities;
	NetEntityId last_netid;
} GameServer;

static GameServer server = {0};



static inline void tcp_send_message(GameServer *server, NetId recipient, NetPackedMessage *packet) {
	server_tcp_send(server->server, recipient, packet->buffer, packet->size);
}
static inline void broadcast(GameServer *server, NetId source, NetPackedMessage *packet) {
	for (size_t i = 0; i < server->clients.count; i++) {
		NetId id = server->clients.items[i].id;
		if (id == source) continue;
		tcp_send_message(server, id, packet);
	}
}


static inline EntityPrefabPropsMiner create_miner_props() {
	Vec2 position = game_spawn_position();
	TextureId text_body = random_range(TEXTURE_MINER_BODY_0000, TEXTURE_MINER_BODY_0005 + 1);
	TextureId text_cloth = random_range(TEXTURE_MINER_CLOTH_0000, TEXTURE_MINER_CLOTH_0005 + 1);
	TextureId text_beard = random_range(TEXTURE_MINER_BEARD_0000, TEXTURE_MINER_BEARD_0005 + 1);
	TextureId text_hat = random_range(TEXTURE_MINER_HAT_0000, TEXTURE_MINER_HAT_0005 + 1);
	return (EntityPrefabPropsMiner){
		.position = position,
		.body = text_body,
		.cloth = text_cloth,
		.beard = text_beard,
		.hat = text_hat,
	};
}


static inline void process_connections(GameServer *server) {
	NetId client_id = server_accept(server->server);
	if (client_id == NET_ID_NONE)
		return;
	da_push(server->clients, ((GameNetClient){
		.id = client_id,
		.ping_timeout = MAX_PING_DELAY,
	}));
	NetEntityId net_entity_id = ++server->last_netid;
	EntityPrefabPropsMiner props = create_miner_props();
	Entity entity = entity_create_miner(game.world, props);
	entity_add_networkentity(game.world, entity, (NetworkEntity){
		.owner = client_id, .id = net_entity_id
	});
	da_push(server->entities, ((NetworkEntityData){
		.id = net_entity_id,
		.entity = entity,
		.owner = client_id,
	}));

	NetMessageEntitySpawnPrefab spawn = {
		.owner = client_id,
		.net_id = net_entity_id,
		.props = props,
	};
	NetPackedMessage packet_spawn = net_message_pack(NET_MESSAGE_ENTITY_SPAWN_PREFAB, &spawn);
	broadcast(server, NET_ID_SERVER, &packet_spawn);

	NetMessagePlayerInit player_init = { .player_entity = net_entity_id, };
	NetPackedMessage packet_player_init = net_message_pack(NET_MESSAGE_PLAYER_INIT, &player_init);
	tcp_send_message(server, client_id, &packet_player_init);

	for (size_t i = 0; i < server->entities.count; i++) {
		NetworkEntityData net_entity_data = server->entities.items[i];
		if (net_entity_data.owner == client_id)
			continue;
		NetMessageEntitySpawnPrefab spawn = {
			.owner = net_entity_data.owner,
			.net_id = net_entity_data.id,
			.props = entity_miner_get_props(game.world, net_entity_data.entity),
		};
		NetPackedMessage packet_spawn = net_message_pack(NET_MESSAGE_ENTITY_SPAWN_PREFAB, &spawn);
		tcp_send_message(server, client_id, &packet_spawn);
	}
}


static inline void remove_client(GameServer *server, GameNetClient *client) {
	printf("[Remove Client] %u", client->id);
	size_t index = 0;
	for (size_t i = 0; i < server->clients.count; i++) {
		if (server->clients.items[i].id == client->id) {
			index = i;
			break;
		}
	}
	assert(index < server->clients.count);
	da_swapremove(server->clients, index);
}


static inline void handle_position_update(
	GameServer *server, GameNetClient *client,
	NetMessageHeader header, void *body
) {
	if (header.length != sizeof(NetMessageEntityPositionUpdate))
		return;
	NetMessageEntityPositionUpdate *update = body;

	Entity entity = network_entity_find(&server->entities, update->net_id);
	Position *position = entity_get_position(game.world, entity);
	*position = update->position;
	NetPackedMessage packet = net_message_pack(NET_MESSAGE_ENTITY_POSITION_UPDATE, update);
	broadcast(server, client->id, &packet);
}


typedef enum {
	NET_READ_NONE,
	NET_READ_OK,
	NET_READ_ERR,
} NetReadResult;

static inline NetReadResult server_try_read(
	GameServer *server, GameNetClient *client,
	NetMessageHeader *header, NetMessageBody *body
) {
	// Peek Header
	NetResult res = server_tcp_peek_into(
		server->server, client->id, header, sizeof(*header)
	);
	if (res.status == NET_ERR)
		return NET_READ_ERR;
	if (res.status == NET_NONE || res.length < sizeof(*header))
		return NET_READ_NONE;

	if (header->length > sizeof(*body)) // Invalid header
		return NET_READ_ERR;
	
	// Peek Body
	ssize_t full_message_length = sizeof(*header) + header->length;
	ssize_t available = server_tcp_peek(server->server, client->id);
	if (available < 0) return NET_READ_ERR;
	if (available < full_message_length) return NET_READ_NONE;

	// Recv Header
	res = server_tcp_recv(server->server, client->id, header, sizeof(*header));
	if (res.status != NET_OK) {
		printf("[NET_READ_ERR] RecvHeader\n");
		return NET_READ_ERR;
	}


	if (header->length > 0) {
		// Recv Body
		res = server_tcp_recv(server->server, client->id, body, header->length);
		if (res.status != NET_OK) {
			printf("[NET_READ_ERR] Recv_Body\n");
			return NET_READ_ERR;
		}
	}
	return NET_READ_OK;
}


static inline void process_client(GameServer *server, GameNetClient *client) {
	if (client->ping_timeout <= 0) {
		printf("Ping timeout failed. Removing client %d\n", client->id);
		remove_client(server, client);
		return;
	}
	client->ping_timeout--;
	static const int MAX_MESSAGES_PER_FRAME = 32;

	int message_index = 0;
	NetMessageHeader header = {0};
	NetMessageBody body = {0};
	while (message_index++ < MAX_MESSAGES_PER_FRAME) {
		NetReadResult result = server_try_read(server, client, &header, &body);
		if (result == NET_READ_NONE) return;
		if (result == NET_READ_ERR) { remove_client(server, client); return; }

		client->ping_timeout = MAX_PING_DELAY;
		switch (header.kind) {
		case NET_MESSAGE_PING: break; // Always updates ping
		case NET_MESSAGE_CLOSE:
			printf("NET_MESSAGE_CLOSE received from %d\n", client->id);
			remove_client(server, client);
			return;
		case NET_MESSAGE_ENTITY_POSITION_UPDATE:
			handle_position_update(server, client, header, &body);
			break;
		default: break;
		}
	}
}

void game_server_init() {
	server = (GameServer){0};
	server.server = server_open(cmd_args.port);
}


void game_server_update() {
	process_connections(&server);
	for (size_t i = 0; i < server.clients.count; i++) {
		process_client(&server, &server.clients.items[i]);
	}
	usleep(NET_DELAY_MS * 1000);
}


void game_server_destroy() {
	server_close(server.server);
	server.server = NULL;
}

