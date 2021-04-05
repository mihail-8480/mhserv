#ifndef MHSERV_MH_MEMORY_H
#define MHSERV_MH_MEMORY_H
#include <stdlib.h>
#include <stdbool.h>
typedef struct mh_memory {
    void* address;
    size_t offset;
    size_t size;
} mh_memory;

mh_memory* mh_memory_new(size_t size, bool clear);
void mh_memory_resize(mh_memory *memory, size_t size);
void mh_memory_free(mh_memory* memory);

#endif //MHSERV_MH_MEMORY_H
