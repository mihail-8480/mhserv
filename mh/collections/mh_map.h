#ifndef MHSERV_MH_MAP_H
#define MHSERV_MH_MAP_H

#include "mh_collection.h"
#include "../mh_memory.h"
typedef struct mh_map {
    mh_collection_t collection;
} mh_map_t;

void mh_map_add(mh_map_t* map, mh_memory_t key, mh_memory_t value);
void mh_map_remove(mh_map_t* map, mh_memory_t key);
mh_memory_t mh_map_get(mh_map_t* map, mh_memory_t key);
bool mh_map_contains(mh_map_t* map, mh_memory_t key);
mh_map_t* mh_map_new(mh_context_t* context);

#endif //MHSERV_MH_MAP_H
