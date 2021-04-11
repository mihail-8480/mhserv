#ifndef MHSERV_MH_MEMORY_H
#define MHSERV_MH_MEMORY_H
#include "mh_context.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// A structure that represents a memory address with a allocation_size and offset
typedef struct mh_memory {
    void* address;
    size_t offset;
    size_t size;
} mh_memory_t;

// Allocate new memory, if clear is true all bytes will be set to 0
mh_memory_t* mh_memory_new(mh_context_t* context, size_t size, bool clear);

// Resize previously allocated memory, to a new allocation_size
void mh_memory_resize(mh_context_t* context, mh_memory_t *memory, size_t size);

// Create a reference to existing memory without allocating anything
mh_memory_t mh_memory_reference(void* address, size_t size);

// Find the index of a character in memory
size_t mh_memory_index_of(mh_memory_t mem, char c);

// Copy memory from the current offset to the character and move the current offset
mh_memory_t mh_memory_read_until(mh_memory_t* mem, char c);

// Create a string
void mh_memory_to_string(char* dest, mh_memory_t mem);

#define MH_REF_CONST(arr) mh_memory_reference(arr, sizeof(arr)-1)
#define MH_REF_STRING(str) mh_memory_reference(str, strlen(str)-1)

#endif //MHSERV_MH_MEMORY_H
