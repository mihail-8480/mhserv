#include "mh_memory.h"
#include "mh_error.h"
#define MEMORY_ERROR(x) mh_error_report("Memory error: " # x)

mh_memory *mh_memory_new(size_t size, bool clear) {
    mh_memory *mem = malloc(size);
    mem->size = size;
    mem->offset = 0;
    if (clear) {
        mem->address = calloc(1, size);
    } else {
        mem->address = malloc(size);
    }
    if (mem->address == NULL) {
        MEMORY_ERROR("Failed allocating memory.");
    }
    return mem;
}

void mh_memory_resize(mh_memory *memory, size_t size) {
    void *new = realloc(memory->address, size);
    if (new == NULL) {
        MEMORY_ERROR("Failed re-allocating memory.");
    }
    memory->address = new;
    memory->size = size;
}

void mh_memory_free(mh_memory *memory) {
    free(memory->address);
    free(memory);
}
