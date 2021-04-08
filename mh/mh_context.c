#include "mh_context.h"
#include <stdlib.h>

// An address that points to allocated memory
typedef void* mh_context_allocation_t;

// The container for the context's variables
typedef struct mh_context_private {
    mh_context_t base;
    size_t allocation_count;
    size_t allocation_size;
    mh_context_allocation_t* allocations;
    size_t destructor_count;
    size_t destructor_size;
    mh_destructor_t** destructors;
    bool (*error_handler)(mh_context_t *, const char *, void *);
} mh_context_private_t;

void mh_destroy(mh_destructor_t *object) {
    // If the object isn't null or it's destructor function pointer isn't null
    if (object != NULL && object->free != NULL) {
        // Call the destructor function pointer
        object->free(object);
    }
}

void mh_context_free(void* ptr) {
    mh_end(ptr);
}

mh_context_t *mh_start(void) {
    // Create the context structure and set some default values
    mh_context_private_t* context = malloc(sizeof(mh_context_private_t));
    *context = (mh_context_private_t){
            .base.destructor = mh_context_free,
            .allocation_count = 0,
            .allocation_size = 32,
            .allocations = malloc(sizeof(mh_context_allocation_t) * 32),
            .destructor_count = 0,
            .destructor_size = 32,
            .destructors = malloc(sizeof(mh_destructor_t*) * 32),
            .error_handler = NULL
    };
    return &context->base;
}

void mh_end(mh_context_t *context) {
    mh_context_private_t* this = (mh_context_private_t*)context;

    // Free every allocation and the allocation array
    for (size_t i = 0; i < this->allocation_count; i++) {
        free(this->allocations[i]);
    }
    free(this->allocations);

    // Call every destructor and free the destructor array
    for (size_t i = 0; i < this->destructor_count; i++) {
        mh_destroy(this->destructors[i]);
    }
    free(this->destructors);

    free(this);
}

void* mh_context_allocate(mh_context_t* context, size_t size, bool clear) {
    mh_context_private_t* this = (mh_context_private_t*)context;

    // Double the allocation array if there isn't enough space
    if (this->allocation_count + 1 >= this->allocation_size) {
        this->allocation_size *= 2;
        this->allocations = realloc(this->allocations, sizeof(mh_context_allocation_t)*this->allocation_size);
    }

    // If clear is true use calloc, if not use malloc
    void *ptr;
    if (!clear) {
        ptr = malloc(size);
    } else {
        ptr = calloc(1, size);
    }

    // Add the pointer to the allocations array
    this->allocations[this->allocation_count++] = ptr;
    return ptr;
}

void* mh_context_add_destructor(mh_context_t *context, mh_destructor_t *destructor) {
    mh_context_private_t* this = (mh_context_private_t*)context;

    // Double the destructor array if there isn't enough space
    if (this->destructor_count + 1 >= this->destructor_size) {
        this->destructor_size *= 2;
        this->destructors = realloc(this->destructors, sizeof(mh_destructor_t*)*this->destructor_size);
    }

    // Add the destructor to the destructors array
    this->destructors[this->destructor_count++] = destructor;
    return destructor;
}

void mh_context_error(mh_context_t *context, const char *message, void *from) {
    mh_context_private_t* this = (mh_context_private_t*)context;

    // If there is an error handler, call it, if it returns true return
    if (this->error_handler != NULL) {
        if (this->error_handler(context,message,from)) {
            return;
        }
    }

    // Destroy the context and crash the program
    mh_end(context);
    abort();
}

void mh_context_set_error_handler(mh_context_t *context, bool (*handler)(mh_context_t *, const char *, void *)) {
    mh_context_private_t* this = (mh_context_private_t*)context;
    this->error_handler = handler;
}
