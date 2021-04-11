#include <string.h>
#include "mh_map.h"

// this code is not worth commenting
typedef struct mh_shitmap {
    mh_map_t base;
    mh_context_t* context;
    mh_memory_t* memory;
    mh_iterator_t* internal_iterator;
} mh_shitmap_t;

typedef struct mh_map_iterator {
    mh_iterator_t base;
    size_t position;
    mh_shitmap_t* map;
} mh_map_iterator_t;

void mh_map_add(mh_map_t* map, mh_memory_t key, mh_memory_t value) {
    MH_THIS(mh_shitmap_t*, map);
    if (this->memory->offset + sizeof(mh_key_value_pair_t) >= this->memory->size) {
        mh_memory_resize(this->context, this->memory, this->memory->size * 2);
    }
    ((mh_key_value_pair_t*)this->memory->address)[this->memory->offset/sizeof(mh_key_value_pair_t)] = (mh_key_value_pair_t){
        .key = key,
        .value = value
    };
    this->memory->offset+=sizeof(mh_key_value_pair_t);
}

void mh_map_remove(mh_map_t* map, mh_memory_t key) {
    MH_THIS(mh_shitmap_t*, map);
    mh_context_error(this->context, "A shitmap cannot remove elements.", mh_map_remove);
}


mh_memory_t mh_map_get(mh_map_t* map, mh_memory_t key) {
    MH_THIS(mh_shitmap_t*, map);
    mh_iterator_start(this->internal_iterator);
    do {
        mh_key_value_pair_t* kv = ((mh_key_value_pair_t*)mh_iterator_current(this->internal_iterator).address);
        if (kv->key.size != key.size) {
            continue;
        }
        if (memcmp(kv->key.address, key.address, key.size) == 0) {
            return kv->value;
        }
    } while(mh_iterator_next(this->internal_iterator));
    return (mh_memory_t){.address = NULL, .size = 0, .offset = 0};
}

bool mh_map_contains(mh_map_t* map, mh_memory_t key) {
    return mh_map_get(map, key).address != NULL;
}

void mh_map_iterator_start(mh_iterator_t* iterator) {
    MH_THIS(mh_map_iterator_t*, iterator);
    this->position = 0;
}

bool mh_map_iterator_next(mh_iterator_t* iterator) {
    MH_THIS(mh_map_iterator_t*, iterator);
    if (sizeof(mh_key_value_pair_t)*(this->position+1) < this->map->memory->offset) {
        this->position++;
        return true;
    }
    return false;
}

mh_memory_t mh_map_iterator_current(mh_iterator_t* iterator) {
    MH_THIS(mh_map_iterator_t*, iterator);
    return mh_memory_reference(&((mh_key_value_pair_t*)this->map->memory->address)[this->position], sizeof (mh_key_value_pair_t));
}
mh_iterator_t* mh_map_get_iterator(mh_collection_t* collection) {
    MH_THIS(mh_shitmap_t*, collection);
    mh_map_iterator_t* iterator = mh_context_allocate(this->context, sizeof(mh_key_value_pair_t), false).ptr;
    *iterator = (mh_map_iterator_t) {
            .base.current = mh_map_iterator_current,
            .base.next = mh_map_iterator_next,
            .base.start = mh_map_iterator_start,
            .map = this,
            .position = 0
    };
    return &iterator->base;
}

mh_map_t* mh_map_new(mh_context_t *context) {
    MH_THIS(mh_shitmap_t*, mh_context_allocate(context, sizeof(mh_shitmap_t), false).ptr);
    mh_memory_t* memory = mh_memory_new(context, sizeof(mh_memory_t)*16, false);
    *this = (mh_shitmap_t) {
            .context = context,
            .memory = memory,
            .base.collection.get_iterator = mh_map_get_iterator,
            .base.collection.destructor = NULL,
    };
    this->internal_iterator = mh_map_get_iterator(&this->base.collection);
    return &this->base;
}
