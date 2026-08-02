#include "client.h"
#include "cmdargs.h"
#include "components.h"
#include "config.h"
#include "darray.h"
#include "entity.h"
#include "entity_id.h"
#include "entity_prefabs.h"
#include "game.h"
#include "netapi.h"
#include "netmessage.h"
#include "network_id.h"
#include "utils.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


typedef struct {
	NetworkId id;
	Entity entity;
} NetworkEntityData;

typedef DynamicArray(NetworkEntityData) NetworkEntities;

typedef struct {
	NetClient *client;
	int ping_delay;
	NetworkEntities entities;
} GameClient;

static pthread_t client_thread;

void game_client_init() {
	pthread_create(&client_thread, NULL, game_client_thread, NULL);
}


static inline void client_ping(GameClient *client) {
	if (client->ping_delay > 0) {
		client->ping_delay--;
		return;
	}
	client->ping_delay = PING_DELAY;
	NetMessageHeader header = {
		.kind = NET_MESSAGE_PING,
	};
	printf("Sending ping\n");
	client_tcp_send(client->client, &header, sizeof(header));
}


void send_update_client_player(GameClient *client) {
	Entity entity = game.player.entity;
	NetMessageHeader header = {
		.kind = NET_MESSAGE_ENTITY_SYNC,
		.message_length = sizeof(NetMessageEntitySync),
	};
	Position *position = entity_get_position(game.world, entity);
	NetworkEntity *netentity = entity_get_networkentity(game.world, entity);
	NetMessageEntitySync body = {
		.entity = *netentity,
		.position = *position,
	};
	printf("Sending message_entity_sync %f;%f\n", position->x, position->y);
	client_tcp_send(client->client, &header, sizeof(header));
	client_tcp_send(client->client, &body, sizeof(NetMessageEntitySync));
}


static inline NetworkEntityData *find_network_entity(GameClient *client, NetworkId id) {
	for (size_t i = 0; i < client->entities.count; i++) {
		if (client->entities.items[i].id != id) continue;
		return &client->entities.items[i];
	}
	da_push(client->entities, ((NetworkEntityData){
		.id = id
	}));
	return &da_last(client->entities);
}


static inline void handle_entity_sync(GameClient *client, NetMessageHeader header) {
	NetMessageEntitySync body = {0};
	printf("Reading handle_entity_sync body\n");
	client_tcp_recv(client->client, &body, header.message_length);
	printf(
		"Sync entity: %d:%d -> %f,%f\n", 
		body.entity.id, body.entity.owner, 
		body.position.x, body.position.y
	);
	NetworkEntityData *net_entity = find_network_entity(client, body.entity.id);
	if (entity_is_alive(game.world, net_entity->entity)) {
		Entity entity = net_entity->entity;
		Position *position = entity_get_position(game.world, entity);
		position->x = body.position.x;
		position->y = body.position.y;
	} else {
		// TODO: We will need to do more syncing, but this is just a quick get-it-working code
		net_entity->entity = entity_create_miner(game.world, body.position);
		Rigidbody *rb = entity_get_rigidbody(game.world, net_entity->entity);
		rb->static_body = true;
	}
}


static inline void handle_message(GameClient *client, NetMessageHeader header) {
	printf("[NET] Header: %d\n", header.kind);
	switch (header.kind) {
		case NET_MESSAGE_CLOSE:
			break;
		case NET_MESSAGE_PING:
			break;
		case NET_MESSAGE_ENTITY_SYNC:
			handle_entity_sync(client, header);
			break;
		case NET_MESSAGE__SIZE:
			UNREACHABLE("NET_MESSAGE__SIZE");
			break;
	}
}

static inline void client_receive(GameClient *client) {
	NetMessageHeader header = {0};
	size_t bytes = client_tcp_peek(client->client);
	if (bytes < sizeof(NetMessageHeader))
		return;
	printf("Reading header\n");
	NetResult res = client_tcp_recv(client->client, &header, sizeof(NetMessageHeader));
	switch (res.status) {
		case NET_NONE: return;
		case NET_OK:
			handle_message(client, header);
			break;
		case NET_CLOSED:
			// TODO:
			break;
		case NET_ERR: break;
	}

}


void *game_client_thread(void *arg) {
	UNUSED(arg);

	GameClient client = {0};
	printf("Trying to connect...\n");
	client.client = client_connect(cmd_args.server_ipdarr, cmd_args.port);
	printf("Connected...\n");

	while (1) {
		client_receive(&client);

		client_ping(&client);
		send_update_client_player(&client);

		usleep(100 * 1000);
	}
	return NULL;
}


void game_client_destroy() {
	// TODO: Send some message to the client thread
	// client_disconnect(client.client);
	// client.client = NULL;
}

