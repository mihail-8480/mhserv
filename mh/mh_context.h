#ifndef MHSERV_MH_CONTEXT_H
#define MHSERV_MH_CONTEXT_H

#include "stdbool.h"
#include "stddef.h"

// Define a variable named this
#define MH_THIS(type, expression) type this = (type)expression

// A function pointer that points to the method that is supposed to free memory
typedef void (*mh_destructor_free_t)(void *ptr);

// A pointer to something allocated
typedef void *mh_context_allocation_t;

// A context reference to allocated memory
typedef struct mh_context_allocation_reference {
    mh_context_allocation_t ptr;
    size_t index;
} mh_context_allocation_reference_t;

// A destructor structure, should be a part of every other structure that needs freeing
typedef struct mh_destructor {
    // The destructor function
    mh_destructor_free_t free;
} mh_destructor_t;

// The context structure, used for error reporting and memory management
typedef struct mh_context {
} mh_context_t;

// A context error handler
typedef bool (*mh_error_handler_t)(mh_context_t *context, const char *message, void *from);

// Destroy a structure that has a mh_destructor as it's first field
void mh_destroy(mh_destructor_t *object);

// Create a context
mh_context_t *mh_start(void);

// End a context
void mh_end(mh_context_t *context);

// Report an error to a context
void mh_context_error(mh_context_t *context, const char *message, void *from);

// Set an error handler to a context
void mh_context_set_error_handler(mh_context_t *context, mh_error_handler_t handler);

// Resize memory in the context
void *mh_context_reallocate(mh_context_t *context, mh_context_allocation_reference_t ref, size_t size);

// Allocate memory that will get destroyed when the context ends
mh_context_allocation_reference_t mh_context_allocate(mh_context_t *context, size_t size, bool clear);

// Add a destructor that will be called when the context ends
void *mh_context_add_destructor(mh_context_t *context, mh_destructor_t *destructor);

// Bind a context to the current thread
void mh_context_bind_to_thread(mh_context_t *context);

// Get a context that was bound to the current thread
mh_context_t *mh_context_get_from_thread(void);

#endif //MHSERV_MH_CONTEXT_H
