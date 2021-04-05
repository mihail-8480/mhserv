#include "include.h"
#include "mh_error.h"

#ifndef MHSERV_MH_BUFFER_H
#define MHSERV_MH_BUFFER_H

// A simple "dynamic" array/buffer
typedef struct {
    size_t size;
    void* ptr;
} mh_buffer;

// Create a new buffer with a size
mh_buffer mh_buffer_new(size_t size) {
    mh_buffer b = {size, calloc(1, size)};
    mh_error_user(b.ptr, "Could not allocate memory", true);
    return b;
}

// Create the buffer by a specific amount
void mh_buffer_increase(mh_buffer* b, size_t size) {
    void *temp = realloc(b->ptr, size);
    mh_error_user(temp, "Could not allocate memory", true);
    b->size = size;
    b->ptr = temp;
}

// Call the previous function, but instead of increasing it by a specific amount you double it
void mh_buffer_double(mh_buffer* b) {
    mh_buffer_increase(b, b->size * 2);
}

// Double if the buffer is smaller than that size
void mh_buffer_auto_double(size_t req_size, mh_buffer* buffer) {
    while(buffer->size < req_size) {
        mh_buffer_double(buffer);
    }
}


#endif //MHSERV_MH_BUFFER_H
