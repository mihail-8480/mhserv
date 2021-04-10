#ifndef MHSERV_MH_MAP_H
#define MHSERV_MH_MAP_H

#include "mh_collection.h"
#include "../mh_memory.h"

// The map structure
typedef struct mh_map {
    mh_collection_t collection;
} mh_map_t;

// The key/value pair structure (the iterator returns mh_memory_t that points to this)
typedef struct mh_key_value_pair {
    mh_memory_t key;
    mh_memory_t value;
} mh_key_value_pair_t;

// Add an element to the map
void mh_map_add(mh_map_t* map, mh_memory_t key, mh_memory_t value);

// Remove an element from the map (WARNING: currently not implemented)
void mh_map_remove(mh_map_t* map, mh_memory_t key);

// Get an element from the map
mh_memory_t mh_map_get(mh_map_t* map, mh_memory_t key);

// Check if the map contains a key
bool mh_map_contains(mh_map_t* map, mh_memory_t key);

// Create a new map inside a context
mh_map_t* mh_map_new(mh_context_t* context);

#endif //MHSERV_MH_MAP_H
