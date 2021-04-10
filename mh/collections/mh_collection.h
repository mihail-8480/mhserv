#ifndef MHSERV_MH_COLLECTION_H
#define MHSERV_MH_COLLECTION_H
#include "../mh_context.h"
#include "../mh_memory.h"

// Something to iterate over
typedef struct mh_iterator {
    void (*start)(struct mh_iterator* iterator);
    bool (*next)(struct mh_iterator* iterator);
    mh_memory_t (*current)(struct mh_iterator* iterator);

} mh_iterator_t;

// A collection
typedef struct mh_collection {
    mh_destructor_t destructor;
    mh_iterator_t* (*get_iterator)(struct mh_collection* collection);
} mh_collection_t;

// Set the iterator to the beginning
void mh_iterator_start(mh_iterator_t* iterator);

// Move to the next element
bool mh_iterator_next(mh_iterator_t* iterator);

// Get the current element
mh_memory_t mh_iterator_current(mh_iterator_t* iterator);

// Create an iterator from a collection
mh_iterator_t *mh_collection_get_iterator(mh_collection_t* collection);


#endif //MHSERV_MH_COLLECTION_H
