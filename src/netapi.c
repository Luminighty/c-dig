#include "netapi.h"
#include "darray.h"
#include <assert.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_LISTEN_COUNT 8
static const uint16_t MAGIC = 0xD19;
static const uint16_t MAGIC_RESP = 0xD04;

typedef struct {
	size_t magic;
	NetId id;
} Handshake;

typedef struct {
	size_t magic;
	NetId id;
	uint16_t port;
} HandshakeResponse;

typedef struct {
	uint16_t port;
	uint32_t ip;
} NetAddress;


static inline struct sockaddr_in address_into_sockaddr(NetAddress address) {
	struct sockaddr_in addr = {0};
	addr.sin_addr.s_addr = htonl(address.ip);
	addr.sin_port = htons(address.port);
	addr.sin_family = AF_INET;
	return addr;
}


static inline NetAddress address_from_sockaddr(struct sockaddr_in addr) {
	NetAddress address = {0};
	address.ip = ntohl(addr.sin_addr.s_addr);
	address.port = ntohs(addr.sin_port);
	return address;
}

typedef struct {
	NetId id;
	NetAddress address;
	int tcp;
	bool ready;
} Connection;
typedef DynamicArray(Connection) Connections;


typedef struct net_server {
	int tcp;
	int udp;
	NetId last_id;
	NetAddress address;
	Connections connections;
} NetServer;


static inline NetResult net_closed()       { return (NetResult){ .status = NET_CLOSED,.length = 0 }; }
static inline NetResult net_none()         { return (NetResult){ .status = NET_NONE,  .length = 0 }; }
static inline NetResult net_ok(size_t len) { return (NetResult){ .status = NET_OK,    .length = len }; }
static inline NetResult net_err()          { return (NetResult){ .status = NET_ERR,   .length = 0 }; }

static inline bool netaddress_eq(NetAddress a, NetAddress b) {
	return a.ip == b.ip && a.port == b.port;
}


static inline Connection *find_connection(NetServer *server, NetId id) {
	for (size_t i = 1; i < server->connections.count; i++) {
		if (server->connections.items[i].id == id)
			return &server->connections.items[i];
	}
	return NULL;
}


static inline size_t find_connection_idx(NetServer *server, NetId id) {
	for (size_t i = 1; i < server->connections.count; i++) {
		if (server->connections.items[i].id == id)
			return i;
	}
	return 0;
}

static inline void on_closed(NetServer *server, NetId id) {
	size_t idx = find_connection_idx(server, id);
	close(server->connections.items[idx].tcp);
	da_swapremove(server->connections, idx);
}



static inline Connection *find_connection_by_address(NetServer *server, NetAddress address) {
	for (size_t i = 1; i < server->connections.count; i++) {
		Connection *conn = &server->connections.items[i];
		if (netaddress_eq(conn->address, address))
			return conn;
	}
	return NULL;
}


static inline void set_nonblocking(int socket) {
	int flags = fcntl(socket, F_GETFL, 0);
	fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}


static inline void set_socket_reuse(int socket) {
	int opt = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
		abort();
	}
}


NetServer *server_open(uint16_t port) {
	NetServer *server = malloc(sizeof(NetServer));
	memset(server, 0, sizeof(NetServer));

	// nil connection
	da_push(server->connections, (Connection){0});

	server->udp = socket(AF_INET, SOCK_DGRAM, 0);
	server->tcp = socket(AF_INET, SOCK_STREAM, 0);

	set_socket_reuse(server->tcp);
	set_socket_reuse(server->udp);

	set_nonblocking(server->tcp);
	set_nonblocking(server->udp);

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	// WARN: we should probably handle this gracefully
	assert(bind(server->udp, (struct sockaddr*)&addr, sizeof(addr)) == 0);
	assert(bind(server->tcp, (struct sockaddr*)&addr, sizeof(addr)) == 0);

	server->address = address_from_sockaddr(addr);

	assert(listen(server->tcp, MAX_LISTEN_COUNT) == 0);

	return server;
}


void server_close(NetServer *server) {
	if (server == NULL) return;
	if (server->tcp >= 0) close(server->tcp);
	if (server->udp >= 0) close(server->udp);
	// TODO: Disconnect clients gracefully
	
	for (size_t i = 0; i < server->connections.count; i++) {
		Connection *conn = &server->connections.items[i];
		if (conn->tcp >= 0) close(conn->tcp);
	}
	da_free(server->connections);
	memset(server, 0, sizeof(NetServer));
	free(server);
}


NetId server_accept(NetServer *server) {
	struct sockaddr_in addr = {0};
	socklen_t len = sizeof(addr);
	int client_fd = accept(server->tcp, (struct sockaddr *)&addr, &len);
	if (client_fd >= 0) {
		NetId id = ++server->last_id;
		Connection conn = {
			.id = id,
			.address = address_from_sockaddr(addr),
			.tcp = client_fd,
		};
		da_push(server->connections, conn);
		set_nonblocking(client_fd);
		
		// NOTE: We should validate that this gets sent
		Handshake handshake = { .id = id, .magic = MAGIC };
		server_tcp_send(server, id, &handshake, sizeof(Handshake));

		return id;
	}

	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return NET_ID_NONE;
	perror("server_accept error");
	return NET_ID_NONE;
}


void server_disconnect(NetServer *server, NetId client_id) {
	size_t i = find_connection_idx(server, client_id);
	if (i == 0) return;
	Connection *client = &server->connections.items[i];

	// TODO: Send disconnect message
	
	close(client->tcp);
	da_swapremove(server->connections, i);
}


NetResult server_tcp_send(NetServer *server, NetId recipient, const void *message, size_t length) {
	Connection *client = find_connection(server, recipient);
	if (client == NULL) return net_none();
	ssize_t n = send(client->tcp, message, length, MSG_NOSIGNAL);
	if (n >= 0)
		return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	perror("server_tcp_send");
	return net_err();
}


NetResult server_tcp_recv(NetServer *server, NetId sender, void *buffer, size_t length) {
	Connection *client = find_connection(server, sender);
	if (client == NULL) return net_none();

	// TODO: Clean this up a little bit, but its fine for now as a quick handshake
	if (!client->ready) {
		HandshakeResponse response = {0};
		ssize_t n = recv(client->tcp, &response, sizeof(response), MSG_NOSIGNAL);
		if (n == 0) {
			on_closed(server, sender);
			return net_closed();
		}
		if (n == sizeof(response)) {
			if (response.magic != MAGIC_RESP || response.id != sender) {
				server_disconnect(server, sender);
				return net_closed();
			}
			client->address.port = response.port;
			client->ready = true;
		}
		return net_none();
	}

	ssize_t n = recv(client->tcp, buffer, length, MSG_NOSIGNAL);
	if (n == 0) {
		on_closed(server, sender);
		return net_closed();
	}
	if (n > 0)
		return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	perror("server_tcp_recv");
	return net_err();
}


// size_t server_tcp_broadcast(NetServer *server, const void *message, size_t length) {
// 	// TODO: NOT IMPLEMENTED
// 	assert(0);
// }


NetResult server_udp_send(NetServer *server, NetId recipient, const void *message, size_t length) {
	Connection *client = find_connection(server, recipient);
	if (client == NULL) return net_closed();
	struct sockaddr_in addr = address_into_sockaddr(client->address);
	ssize_t n = sendto(server->udp, message, length, 0, (struct sockaddr*)&addr, sizeof(addr));
	if (n >= 0)
		return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	return net_err();
}


NetResult server_udp_recv(NetServer *server, void *buffer, size_t length, NetId *sender) {
	struct sockaddr_in addr = {0};
	socklen_t len = sizeof(addr);
	ssize_t n = recvfrom(server->udp, buffer, length, 0, (struct sockaddr*)&addr, &len);
	if (n < 0 && (errno == EWOULDBLOCK || errno == EAGAIN))
		return net_none();

	printf("[UDP_RECV] %zd\n", n);
	NetAddress address = address_from_sockaddr(addr);
	printf("[UDP_RECV] %d %d\n", address.ip, address.port);

	Connection *client = find_connection_by_address(server, address);
	if (client == NULL) {
		Handshake* handshake = buffer;
		size_t index = find_connection_idx(server, handshake->id);
		if (handshake->magic == MAGIC && index != 0) {
			server->connections.items[index].address = address;
		}
		return net_none();
	}
	*sender = client->id;
	if (n == 0) return net_none();
	if (n > 0)
		return net_ok(n);
	perror("server_udp_recv");
	return net_err();
}


// size_t server_udp_broadcast(NetServer *server, const void *message, size_t length) {
// 	// TODO: NOT IMPLEMENTED
// 	assert(0);
// }


typedef struct net_client {
	int tcp;
	int udp;
	NetAddress server;
	NetId id;
} NetClient;


NetClient *client_connect(const char *ipaddr, uint16_t port) {
	NetClient *client = malloc(sizeof(NetClient));
	memset(client, 0, sizeof(NetClient));

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(port);

	client->udp = socket(AF_INET, SOCK_DGRAM, 0);
	client->tcp = socket(AF_INET, SOCK_STREAM, 0);
	client->server.port = port;
	client->server.ip = ntohl(addr.sin_addr.s_addr);

	// WARN: we should probably handle this gracefully
	// NOTE: connect udp lets me check the sock port
	assert(connect(client->udp, (struct sockaddr *)&addr, sizeof(addr)) == 0);
	assert(connect(client->tcp, (struct sockaddr *)&addr, sizeof(addr)) == 0);

	client->server = address_from_sockaddr(addr);

	struct sockaddr_in self_addr = {0};
	socklen_t len = sizeof(self_addr);
	assert(getsockname(client->udp, (struct sockaddr *)&self_addr, &len) == 0);
	NetAddress self_address = address_from_sockaddr(self_addr);

	Handshake handshake = {0};
	client_tcp_recv(client, &handshake, sizeof(handshake));
	assert(handshake.magic == MAGIC);
	client->id = handshake.id;
	HandshakeResponse response = {
		.magic = MAGIC_RESP,
		.id = handshake.id,
		port = self_address.port
	};
	client_tcp_send(client, &response, sizeof(HandshakeResponse));

	return client;
}


void client_disconnect(NetClient *client) {
	if (client == NULL) return;
	if (client->tcp >= 0) close(client->tcp);
	if (client->udp >= 0) close(client->udp);
	memset(client, 0, sizeof(NetClient));
	free(client);
}


NetResult client_tcp_send(NetClient *client, const void *message, size_t length) {
	ssize_t n = send(client->tcp, message, length, 0);
	if (n >= 0)
		return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	perror("client_tcp_send");
	return net_err();
}


NetResult client_tcp_recv(NetClient *client, void *buffer, size_t length) {
	ssize_t n =  recv(client->tcp, buffer, length, MSG_NOSIGNAL);
	if (n == 0) return net_closed();
	if (n > 0) return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	perror("server_tcp_recv");
	return net_err();
}


NetResult client_udp_send(NetClient *client, const void *message, size_t length) {
	struct sockaddr_in addr = address_into_sockaddr(client->server);
	ssize_t n = sendto(client->udp, message, length, 0, (struct sockaddr*)&addr, sizeof(addr));
	if (n >= 0)
		return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	return net_err();
}


NetResult client_udp_recv(NetClient *client, void *buffer, size_t length) {
	struct sockaddr_in addr = {0};
	socklen_t len = sizeof(addr);
	ssize_t n = recvfrom(client->udp, buffer, length, 0, (struct sockaddr*)&addr, &len);
	NetAddress address = address_from_sockaddr(addr);

	// TODO: Might have to handle this
	assert(netaddress_eq(address, client->server));

	if (n == 0) return net_none();
	if (n > 0)
		return net_ok(n);
	if (errno == EWOULDBLOCK || errno == EAGAIN)
		return net_none();
	perror("server_udp_recv");
	return net_err();
}

