#include "server.h"
#include "config.h"
#include "darray.h"
#include "entity_id.h"
#include "utils.h"
#include "netapi.h"
#include "netmessage.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_PING_DELAY (PING_DELAY * 3)

typedef struct {
	NetId id;
	int ping_timeout;
	Entity player;
} GameClient;

typedef DynamicArray(GameClient) GameClients;

typedef struct {
	NetServer *server;
	GameClients clients;
} GameServer;

static pthread_t server_thread;

void game_server_init() {
	pthread_create(&server_thread, NULL, game_server_thread, NULL);
}


static inline void process_connections(GameServer *server) {
	NetId id = server_accept(server->server);
	if (id == NET_ID_NONE)
		return;
	da_push(server->clients, ((GameClient){
		.id = id,
		.ping_timeout = MAX_PING_DELAY,
	}));
	printf("Client connected: %d\n", id);
}


static inline void remove_client(GameServer *server, GameClient *client) {
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

static inline void broadcast(GameServer *server, NetId source, NetMessageHeader header, void *body) {
	for (size_t i = 0; i < server->clients.count; i++) {
		NetId id = server->clients.items[i].id;
		if (id == source) continue;
		server_tcp_send(server->server, id, &header, sizeof(header));
		server_tcp_send(server->server, id, body, header.message_length);
	}
}


static inline void handle_entity_sync(GameServer *server, GameClient *client, NetMessageHeader header) {
	NetMessageEntitySync body = {0};
	NetResult result = server_tcp_recv(server->server, client->id, &body, header.message_length);
	// printf(
	// 	"Sync entity: %d:%d -> %f,%f (status: %d, bytes %zu/%zu)\n", 
	// 	body.entity.id, body.entity.owner, 
	// 	body.position.x, body.position.y,
	// 	result.status, result.length, header.message_length
	// );
	broadcast(server, client->id, header, &body);
}


static inline void handle_message(GameServer *server, GameClient *client, NetMessageHeader header) {
	switch (header.kind) {
		case NET_MESSAGE_CLOSE:
			printf("NET_MESSAGE_CLOSE received from %d\n", client->id);
			remove_client(server, client);
			break;
		case NET_MESSAGE_PING:
			client->ping_timeout = MAX_PING_DELAY;
			printf("PINGED %d\n", client->id);
			break;
		case NET_MESSAGE_ENTITY_SYNC:
			handle_entity_sync(server, client, header);
			break;
		case NET_MESSAGE__SIZE:
			UNREACHABLE("NET_MESSAGE__SIZE");
			break;
			
	}
}


static inline void process_client(GameServer *server, GameClient *client) {
	if (client->ping_timeout <= 0) {
		printf("Ping timeout failed. Removing client %d\n", client->id);
		remove_client(server, client);
	}
	client->ping_timeout--;
	static const int MAX_MESSAGES = 32;

	NetId sender = client->id;
	int message_index = 0;
	while (message_index++ < MAX_MESSAGES) {
		NetMessageHeader header = {0};
		size_t bytes = server_tcp_peek(server->server, client->id);
		// printf("Bytes available %zu/%zu\n", bytes, sizeof(header));
		if (bytes < sizeof(header))
			return;
		// printf("Reading header from %d\n", client->id);
		NetResult res = server_tcp_recv(server->server, sender, &header, sizeof(header));
		switch (res.status) {
			case NET_NONE:
				return;
			case NET_OK:
				client->ping_timeout = MAX_PING_DELAY;
				handle_message(server, client, header);
				break;
			case NET_CLOSED:
				printf("Socket closed unexpectedly. Closing %d\n", client->id);
				remove_client(server, client);
				break;
			case NET_ERR: break;
		}
	}
}


// static inline void process_udp() {
// 	NetMessageHeader header = {0};
// 	static const int MAX_MESSAGES = 128;
// 	int message_index = 0;
// 	while (message_index++ < MAX_MESSAGES) {
// 		NetId sender = 0;
// 		NetResult res = server_udp_recv(
// 			server.server, &header, sizeof(header), &sender
// 		);
// 		// printf("[UDP] %d %zu\n", res.status, res.length);
// 		switch (res.status) {
// 			case NET_NONE: return;
// 			case NET_OK: handle_message(sender, header); break;
// 			case NET_CLOSED: remove_client(sender); break;
// 			case NET_ERR: break;
// 		}
// 	}
// }


void *game_server_thread(void *arg) {
	UNUSED(arg);
	GameServer server = {0};
	server.server = server_open(DEFAULT_PORT);
	while (1) {
		// printf("[SERVER UPDATE] %zu\n", server.clients.count);
		process_connections(&server);
		for (size_t i = 0; i < server.clients.count; i++) {
			process_client(&server, &server.clients.items[i]);
		}
		usleep(NET_DELAY_MS * 1000);
	}
	// process_udp();
}


void game_server_destroy() {
	// Send message to server thread
	// server_close(server.server);
	// server.server = NULL;
}

