#ifndef MHSERV_MH_COLLECTION_H
#define MHSERV_MH_COLLECTION_H
#include "../mh_context.h"
#include "../mh_memory.h"

typedef struct mh_iterator {
    void (*start)(struct mh_iterator* iterator);
    bool (*next)(struct mh_iterator* iterator);
    mh_memory_t (*current)(struct mh_iterator* iterator);

} mh_iterator_t;

typedef struct mh_collection {
    mh_destructor_t destructor;
    mh_iterator_t* (*get_iterator)(struct mh_collection* collection);
} mh_collection_t;

void mh_iterator_start(mh_iterator_t* iterator);

bool mh_iterator_next(mh_iterator_t* iterator);

mh_memory_t mh_iterator_current(mh_iterator_t* iterator);

mh_iterator_t *mh_collection_get_iterator(mh_collection_t* collection);


#endif //MHSERV_MH_COLLECTION_H
