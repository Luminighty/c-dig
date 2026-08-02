#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>


typedef enum {
	NET_NONE,
	NET_OK,
	NET_ERR,
	NET_CLOSED,
} NetResultStatus;

typedef struct {
	NetResultStatus status;
	size_t length;
} NetResult;


typedef uint32_t NetId;
#define NET_ID_NONE 0

typedef struct net_server NetServer;

NetServer *server_open(uint16_t port);
void server_close(NetServer *server);
NetId server_accept(NetServer *server);
void server_disconnect(NetServer *server, NetId client);

// WARN: Return values are suspect to change, as we may need to report errors and such
NetResult server_tcp_send(NetServer *server, NetId recipient, const void *message, size_t length);
NetResult server_tcp_recv(NetServer *socket, NetId sender, void *buffer, size_t length);
// size_t server_tcp_broadcast(NetServer *server, const void *message, size_t length);
ssize_t server_tcp_peek(NetServer *server, NetId recipient);

NetResult server_udp_send(NetServer *server, NetId recipient, const void *message, size_t length);
NetResult server_udp_recv(NetServer *server, void *buffer, size_t length, NetId *sender);
// size_t server_udp_broadcast(NetServer *server, const void *message, size_t length);


typedef struct net_client NetClient;

NetClient *client_connect(const char *ipaddr, uint16_t port);
void client_disconnect(NetClient *client);

NetResult client_tcp_send(NetClient *client, const void *message, size_t length);
NetResult client_tcp_recv(NetClient *client, void *buffer, size_t length);
ssize_t client_tcp_peek(NetClient *client);

NetResult client_udp_send(NetClient *client, const void *message, size_t length);
NetResult client_udp_recv(NetClient *client, void *buffer, size_t length);


#endif // NETWORKAPI_H
