#ifndef MHSERV_MH_DESTRUCTOR_H
#define MHSERV_MH_DESTRUCTOR_H
#include <stddef.h>

// A destructor structure, should be a part of every other structure that needs freeing
typedef struct mh_destructor {
    void (*free)(void* ptr);
} mh_destructor_t;

// Destroy a structure
void mh_destructor_free(void *ptr);

// Create an array of destructors
mh_destructor_t* mh_destructor_array_new(mh_destructor_t** array, size_t length);

#endif //MHSERV_MH_DESTRUCTOR_H
