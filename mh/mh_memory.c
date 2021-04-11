#include "mh_memory.h"

typedef struct mh_memory_private {
    mh_memory_t base;
    size_t index;
} mh_memory_private_t;


mh_memory_t *mh_memory_new(mh_context_t* context, size_t size, bool clear) {
    // Allocate the memory container and set it's fields
    MH_THIS(mh_memory_private_t*, mh_context_allocate(context, sizeof(mh_memory_private_t), false).ptr);
    if (this == NULL) {
        mh_context_error(context, "Couldn't allocate memory for the structure.", mh_memory_new);
        return NULL;
    }

    mh_context_allocation_reference_t ref = mh_context_allocate(context, size, clear);
    this->base.size = size;
    this->base.offset = 0;
    this->base.address = ref.ptr;
    this->index = ref.index;
    return &this->base;
}

void mh_memory_resize(mh_context_t* context, mh_memory_t *memory, size_t size) {
    // A re-alloc isn't needed if the allocation_size is smaller than the memory allocation_size
    MH_THIS(mh_memory_private_t *, memory);

    if (size <= this->base.size) {
        return;
    }


    // Reallocate to a new pointer to avoid memory leaks on error
    void *new = mh_context_reallocate(context,
                                      (mh_context_allocation_reference_t) {
                                              .index = this->index,
                                              .ptr = this->base.address
                                      }, size);

    // If the new pointer is null, report the error
    if (new == NULL) {
        mh_context_error(context, "Couldn't resize the memory.", mh_memory_resize);
        return;
    }

    // Set the new address and allocation_size
    this->base.address = new;
    this->base.size = size;
}

mh_memory_t mh_memory_reference(void *address, size_t size) {
    // Create a new instance of the structure with offset 0
    return (mh_memory_t){.address = address, .size = size, .offset = 0};
}

mh_memory_t mh_memory_read_until(mh_memory_t *memory, char c) {

    // Copy the memory into a c-string from the current offset, to the index of the character
    size_t index = mh_memory_index_of(*memory,c);
    if (index == -1) {
        return mh_memory_reference(NULL, 0);
    }
    size_t size = index-memory->offset;

    mh_memory_t ref = mh_memory_reference(memory->address+memory->offset, size);

    // Move the offset forward
    memory->offset += size+1;
    return ref;
}

size_t mh_memory_index_of(mh_memory_t memory, char c) {
    char* str = (char*)memory.address;

    // Find the index of a character in the memory
    for(size_t i = memory.offset; i < memory.size; i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;
}

void mh_memory_to_string(char *dest, mh_memory_t mem) {
    memcpy(dest, mem.address, mem.size);
    dest[mem.size] = 0;
}

bool mh_memory_is_equal(mh_memory_t first, mh_memory_t second) {
    if (first.size != second.size) return false;
    return memcmp(first.address, second.address, first.size) == 0;
}
