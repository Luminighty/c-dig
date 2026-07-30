#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include <stddef.h>
#include <stdint.h>


typedef enum {
	NET_MESSAGE_CLOSE = 0,
	NET_MESSAGE_PING,
} NetMessageKind;


typedef struct {
	NetMessageKind kind;
	size_t message_length;
} NetMessageHeader;

typedef struct {
	NetMessageHeader header;
	uint8_t *message;
	size_t index;
	size_t capacity;
} NetMessageReader;


#endif // NET_MESSAGE_H
