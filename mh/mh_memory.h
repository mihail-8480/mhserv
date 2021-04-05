#ifndef MHSERV_MH_MEMORY_H
#define MHSERV_MH_MEMORY_H
#include <stdlib.h>
#include <stdbool.h>
typedef struct mh_memory {
    void* address;
    size_t offset;
    size_t size;
} mh_memory_t;

mh_memory_t* mh_memory_new(size_t size, bool clear);
void mh_memory_resize(mh_memory_t *memory, size_t size);
void mh_memory_free(mh_memory_t* memory);
mh_memory_t mh_memory_reference(void* address, size_t size);
#endif //MHSERV_MH_MEMORY_H
