#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include <stddef.h>


typedef enum {
	NET_MESSAGE_CLOSE = 0,
	NET_MESSAGE_PING,
} NetMessageKind;


typedef struct {
	NetMessageKind kind;
	size_t message_length;
} NetMessageHeader;


#endif // NET_MESSAGE_H
