#ifndef MHSERV_MH_CONTEXT_H
#define MHSERV_MH_CONTEXT_H
#include "stdbool.h"
#include "stddef.h"


// A destructor structure, should be a part of every other structure that needs freeing
typedef struct mh_destructor {
    void (*free)(void* ptr);
} mh_destructor_t;

typedef struct mh_context {
    mh_destructor_t destructor;
} mh_context_t;


// Destroy a structure that inherits mh_destructor
void mh_destroy(void *ptr);

mh_context_t* mh_start(void);
void mh_end(mh_context_t *context);
void mh_context_error(mh_context_t* context, const char* message, void* from);
void mh_context_set_error_handler(mh_context_t* context, bool (*handler)(mh_context_t* context, const char* message, void* from));
void* mh_context_allocate(mh_context_t* context, size_t size, bool clear);
void* mh_context_add_destructor(mh_context_t* context, mh_destructor_t* destructor);
#endif //MHSERV_MH_CONTEXT_H
