#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include "components.h"
#include <stddef.h>
#include <stdint.h>


typedef enum {
	NET_MESSAGE_CLOSE = 1,
	NET_MESSAGE_PING,
	NET_MESSAGE_ENTITY_SYNC,
	NET_MESSAGE__SIZE,
} NetMessageKind;


typedef struct {
	NetMessageKind kind;
	size_t message_length;
} NetMessageHeader;



typedef struct {
	NetworkEntity entity;
	Position position;
} NetMessageEntitySync;


typedef struct {
	NetMessageHeader header;
	uint8_t *message;
	size_t index;
	size_t capacity;
} NetMessageReader;


#endif // NET_MESSAGE_H
