#include "mh_collection.h"

mh_iterator_t *mh_collection_get_iterator(mh_collection_t *collection) {
    return collection->get_iterator(collection);
}

void mh_iterator_start(mh_iterator_t *iterator) {
    iterator->start(iterator);
}

bool mh_iterator_next(mh_iterator_t *iterator) {
    return iterator->next(iterator);
}

mh_memory_t mh_iterator_current(mh_iterator_t *iterator) {
    return iterator->current(iterator);
}