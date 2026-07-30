#ifndef NET_ENTITY_H
#define NET_ENTITY_H


#include "darray.h"
#include "network_id.h"


typedef DynamicArray(NetworkId) NetworkIds;
typedef struct {
	NetworkIds ids;

} NetworkEntities;



#endif // NET_ENTITY_H
