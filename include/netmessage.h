#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include "components.h"
#include "entity_prefabs.h"
#include "linalg.h"
#include "netentity_id.h"
#include "tile.h"
#include <stddef.h>
#include <stdint.h>


typedef enum {
	NET_MESSAGE_CLOSE = 1,
	NET_MESSAGE_PING,
	NET_MESSAGE_PLAYER_INIT,
	NET_MESSAGE_ENTITY_SPAWN_PREFAB,
	NET_MESSAGE_ENTITY_POSITION_UPDATE,
	NET_MESSAGE_MAP_UPDATE,
} NetMessageKind;


typedef struct {
	uint32_t kind;
	uint32_t length;
} NetMessageHeader;

#define NET_MESSAGE_MAX_BODY 16384
typedef struct {
	uint8_t buffer[NET_MESSAGE_MAX_BODY];
} NetMessageBody;


typedef struct {
	// TODO: EntityPrefabID prefab;
	uint32_t owner;
	NetEntityId net_id;
	EntityPrefabPropsMiner props;
} NetMessageEntitySpawnPrefab;


typedef struct {
	NetEntityId player_entity;
} NetMessagePlayerInit;


typedef struct {
	NetEntityId net_id;
	Position position;
} NetMessageEntityPositionUpdate;


typedef struct {
	Tile tile;
	Rect2i rect;
} NetMessageMapUpdate;

typedef struct {
	uint8_t buffer[sizeof(NetMessageHeader) + NET_MESSAGE_MAX_BODY];
	size_t size;
} NetPackedMessage;

#define net_message_pack(kind, body) _net_message_pack((kind), (body), sizeof(*(body)))
static inline NetPackedMessage _net_message_pack(NetMessageKind kind, void *body, size_t body_size) {
	NetPackedMessage packet;
	NetMessageHeader header = { .kind = kind, .length = body_size };
	assert(header.length <= NET_MESSAGE_MAX_BODY);
	memcpy(packet.buffer, &header, sizeof(header));
	memcpy(packet.buffer + sizeof(header), body, header.length);
	packet.size = sizeof(header) + header.length;
	return packet;
}

#endif // NET_MESSAGE_H
