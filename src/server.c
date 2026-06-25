#include "server.h"
#include "config.h"
#include "darray.h"
#include "netapi.h"
#include "netmessage.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

typedef DynamicArray(NetId) NetIds;

typedef struct {
	NetServer *server;
	NetIds clients;
} GameServer;

GameServer server = {0};

void game_server_init() {
	server.server = server_open(DEFAULT_PORT);
}

static inline void process_connections() {
	NetId id = server_accept(server.server);
	if (id == NET_ID_NONE)
		return;
	da_push(server.clients, id);
	printf("Client connected: %d\n", id);
}

static inline void remove_client(NetId client) {
	size_t index = 0;
	for (size_t i = 0; i < server.clients.count; i++) {
		if (server.clients.items[i] == client)
			index = i;
	}
	da_swapremove(server.clients, index);
}

static inline void handle_message(NetId sender, NetMessageHeader header) {
	switch (header.kind) {
		case NET_MESSAGE_CLOSE: remove_client(sender); break;
		case NET_MESSAGE_PING: printf("PINGED %d\n", sender); break;
	}
}

static inline void process_client(NetId sender) {
	NetMessageHeader header = {0};
	static const int MAX_MESSAGES = 32;
	int message_index = 0;
	while (message_index++ < MAX_MESSAGES) {
		NetResult res = server_tcp_recv(server.server, sender, &header, sizeof(header));
		switch (res.status) {
			case NET_NONE: return;
			case NET_OK: handle_message(sender, header); break;
			case NET_CLOSED: remove_client(sender); break;
			case NET_ERR: break;
		}
	}
}

static inline void process_udp() {
	NetMessageHeader header = {0};
	static const int MAX_MESSAGES = 128;
	int message_index = 0;
	while (message_index++ < MAX_MESSAGES) {
		NetId sender = 0;
		NetResult res = server_udp_recv(
			server.server, &header, sizeof(header), &sender
		);
		// printf("[UDP] %d %zu\n", res.status, res.length);
		switch (res.status) {
			case NET_NONE: return;
			case NET_OK: handle_message(sender, header); break;
			case NET_CLOSED: remove_client(sender); break;
			case NET_ERR: break;
		}
	}
}


void game_server_update() {
	// printf("[SERVER UPDATE]\n");
	process_connections();
	for (size_t i = 0; i < server.clients.count; i++) {
		NetId sender = server.clients.items[i];
		process_client(sender);
	}
	process_udp();
}


void game_server_destroy() {
	server_close(server.server);
	server.server = NULL;
}

