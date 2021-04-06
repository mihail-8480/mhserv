#include "mh_destructor.h"
#include <stdlib.h>

typedef struct mh_destructor_array {
    mh_destructor_t destructor;
    mh_destructor_t** array;
    size_t length;
} mh_destructor_array_t;


void mh_destructor_free(void *ptr) {
    mh_destructor_t *self = (mh_destructor_t*)ptr;
    // Call the free method
    self->free(self);
}

void mh_destructor_array_free(void* ptr) {
    mh_destructor_array_t* self = (mh_destructor_array_t*)ptr;
    // Call every destructor
    for(size_t i = 0; i < self->length; i++) {
        if (self->array[i] != NULL) {
            mh_destructor_free(self->array[i]);
        }
    }
    free(ptr);
}

mh_destructor_t *mh_destructor_array_new(mh_destructor_t **array, size_t length) {
    mh_destructor_array_t* ptr = calloc(1, sizeof(mh_destructor_array_t));
    *ptr = (mh_destructor_array_t) {
        .length = length,
        .array = array,
        .destructor.free = mh_destructor_array_free
    };
    return (mh_destructor_t*)ptr;
}

void mh_destructor_array_set(mh_destructor_t *array, size_t index, mh_destructor_t *destructor) {
    mh_destructor_array_t* self = (mh_destructor_array_t*)array;
    self->array[index] = destructor;
}
