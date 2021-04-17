#ifndef MHSERV_MH_HANDLE_H
#define MHSERV_MH_HANDLE_H

#include "mh_context.h"

// A handle to a dynamic library
typedef struct mh_handle {
    mh_destructor_t destructor;
} mh_handle_t;

// Load a library and create a handle
mh_handle_t *mh_handle_new(mh_context_t *context, const char *path);

// Find a symbol inside of a library
void *mh_handle_find_symbol(mh_handle_t *handle, const char *name);

#endif //MHSERV_MH_HANDLE_H
