#include "mh_handle.h"
#include <dlfcn.h>

typedef struct mh_handle_private {
    mh_handle_t base;
    void *handle;
    mh_context_t* context;
} mh_handle_private_t;

void mh_handle_destroy(void* ptr) {
    MH_THIS(mh_handle_private_t*, ptr);
    dlclose(this->handle);
}

mh_handle_t *mh_handle_new(mh_context_t *context, const char *path) {
    MH_THIS(mh_handle_private_t*, (mh_handle_private_t*)mh_context_allocate(context, sizeof(mh_handle_private_t), false).ptr);
    void* handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        mh_context_error(context, dlerror(), mh_handle_new);
        return NULL;
    }
    *this = (mh_handle_private_t) {
        .context = context,
        .base.destructor = mh_handle_destroy,
        .handle = handle
    };
    mh_context_add_destructor(context, &this->base.destructor);
    return (mh_handle_t*)this;
}

void *mh_handle_find_symbol(mh_handle_t *handle, const char *name) {
    MH_THIS(mh_handle_private_t*, handle);
    void *sym = dlsym(this->handle, name);
    if (!sym) {
        mh_context_error(this->context, dlerror(), mh_handle_new);
        return NULL;
    }
    return sym;
}
