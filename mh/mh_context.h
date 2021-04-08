#ifndef MHSERV_MH_CONTEXT_H
#define MHSERV_MH_CONTEXT_H
#include "stdbool.h"
#include "stddef.h"

// A function pointer that points to the method that is supposed to free memory
typedef void (*mh_destructor_free_t)(void* ptr);

// A destructor structure, should be a part of every other structure that needs freeing
typedef struct mh_destructor {
    // The destructor function
    mh_destructor_free_t free;
} mh_destructor_t;

// The context structure, used for error reporting and memory management
typedef struct mh_context {
    // The destructor
    mh_destructor_t destructor;
} mh_context_t;

// A context error handler
typedef bool (*mh_error_handler_t)(mh_context_t* context, const char* message, void* from);

// Destroy a structure that has a mh_destructor as it's first field
void mh_destroy(mh_destructor_t *object);

// Create a context
mh_context_t* mh_start(void);

// End a context
void mh_end(mh_context_t *context);

// Report an error to a context
void mh_context_error(mh_context_t* context, const char* message, void* from);

// Set an error handler to a context
void mh_context_set_error_handler(mh_context_t* context, mh_error_handler_t handler);

// Allocate memory that will get destroyed when the context ends
void* mh_context_allocate(mh_context_t* context, size_t size, bool clear);

// Add a destructor that will be called when the context ends
void* mh_context_add_destructor(mh_context_t* context, mh_destructor_t* destructor);
#endif //MHSERV_MH_CONTEXT_H
