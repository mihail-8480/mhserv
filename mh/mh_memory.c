#include "mh_memory.h"

void mh_memory_free(void* memory) {
    // Free the memory
    free(((mh_memory_t*)memory)->address);
    free(memory);
}

mh_memory_t *mh_memory_new(mh_context_t* context, size_t size, bool clear) {
    // Allocate the memory container and set it's fields
    mh_memory_t *mem = malloc(sizeof(mh_memory_t));
    if (mem == NULL) {
        mh_context_error(context, "Couldn't allocate memory for the structure.", mh_memory_new);
        return NULL;
    }
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
        free(mem);
        mh_context_error(context, "Couldn't allocate memory.", mh_memory_new);
        return NULL;
    }
    return mem;
}

void mh_memory_resize(mh_context_t* context, mh_memory_t *memory, size_t size) {
    // A re-alloc isn't needed if the allocation_size is smaller than the memory allocation_size
    if (size <= memory->size) {
        return;
    }
    // Reallocate to a new pointer to avoid memory leaks on error
    void *new = realloc(memory->address, size);

    // If the new pointer is null, report the error
    if (new == NULL) {
        mh_context_error(context, "Couldn't resize the memory.", mh_memory_resize);
        return;
    }

    // Set the new address and allocation_size
    memory->address = new;
    memory->size = size;
}

mh_memory_t mh_memory_reference(void *address, size_t size) {
    // Create a new instance of the structure with offset 0
    return (mh_memory_t){.address = address, .size = size, .offset = 0, .destructor.free = NULL};
}

mh_memory_t mh_memory_read_until(mh_memory_t *mem, char c) {

    // Copy the memory into a c-string from the current offset, to the index of the character
    size_t index = mh_memory_index_of(mem,c);
    if (index == -1) {
        return mh_memory_reference(NULL, 0);
    }
    size_t size = index-mem->offset;

    mh_memory_t ref = mh_memory_reference(mem->address+mem->offset, size);

    // Move the offset forward
    mem->offset += size+1;
    return ref;
}

size_t mh_memory_index_of(mh_memory_t *mem, char c) {
    char* str = (char*)mem->address;

    // Find the index of a character in the memory
    for(size_t i = mem->offset; i < mem->size; i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;
}
