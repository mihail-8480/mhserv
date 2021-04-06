#include "mh_memory.h"
#include "mh_error.h"

// Report a memory error
#define MEMORY_ERROR(x) mh_error_report("Memory error: " # x)

void mh_memory_free(void* memory) {
    // Free the memory
    free(((mh_memory_t*)memory)->address);
    free(memory);
}


mh_memory_t *mh_memory_new(size_t size, bool clear) {
    // Allocate the memory container and set it's fields
    mh_memory_t *mem = malloc(size);
    mem->size = size;
    mem->offset = 0;
    mem->destructor.free = mh_memory_free;

    // If clear is true use calloc (zero-everything)
    if (clear) {
        mem->address = calloc(1, size);
    } else {
        // If clear is false use malloc
        mem->address = malloc(size);
    }

    // If the memory wasn't properly allocated, report the error
    if (mem->address == NULL) {
        MEMORY_ERROR("Failed allocating memory.");
    }
    return mem;
}

void mh_memory_resize(mh_memory_t *memory, size_t size) {
    // Reallocate to a new pointer to avoid memory leaks on error
    void *new = realloc(memory->address, size);

    // If the new pointer is null, report the error
    if (new == NULL) {
        MEMORY_ERROR("Failed re-allocating memory.");
    }

    // Set the new address and size
    memory->address = new;
    memory->size = size;
}

mh_memory_t mh_memory_reference(void *address, size_t size) {
    // Create a new instance of the structure with offset 0
    return (mh_memory_t){.address = address, .size = size, .offset = 0};
}
