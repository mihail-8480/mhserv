#ifndef MHSERV_MH_MEMORY_H
#define MHSERV_MH_MEMORY_H
#include <stdlib.h>
#include <stdbool.h>

// A structure that represents a memory address with a size and offset
typedef struct mh_memory {
    void* address;
    size_t offset;
    size_t size;
} mh_memory_t;

// Allocate new memory, if clear is true all bytes will be set to 0
mh_memory_t* mh_memory_new(size_t size, bool clear);

// Resize previously allocated memory, to a new size
void mh_memory_resize(mh_memory_t *memory, size_t size);

// Free the used memory
void mh_memory_free(mh_memory_t* memory);

// Create a reference to existing memory without allocating anything
mh_memory_t mh_memory_reference(void* address, size_t size);
#endif //MHSERV_MH_MEMORY_H
