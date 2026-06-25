#include "client.h"
#include "config.h"
#include "netapi.h"
#include "netmessage.h"
#include <stdio.h>


typedef struct {
	NetClient *client;
} GameClient;

GameClient client = {0};

void game_client_init() {
	printf("Trying to connect...\n");
	client.client = client_connect("0.0.0.0", DEFAULT_PORT);
}


void game_client_update() {
	// client_tcp_send(client.client, "Test", size_t length)
}

void game_client_destroy() {
	client_disconnect(client.client);
	client.client = NULL;
}

void game_client_send_ping() {
	NetMessageHeader header = {
		.kind = NET_MESSAGE_PING,
	};
	client_tcp_send(client.client, &header, sizeof(header));
}

