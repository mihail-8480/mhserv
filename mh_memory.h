#include "include.h"
#include "mh_buffer.h"

#ifndef MHSERV_MH_MEMORY_H
#define MHSERV_MH_MEMORY_H
typedef struct {
    mh_buffer buffer;
    size_t position;
} mh_memory;

mh_memory mh_memory_new(int size) {
    mh_buffer buffer = mh_buffer_new(size);
    size_t position = 0;
    return (mh_memory){buffer, position};
}

void mh_memory_write(mh_memory *output, const void* data, size_t size) {
    while (size + output->position >= output->buffer.size) {
        mh_buffer_double(&output->buffer);
    }
    memcpy(output->buffer.ptr + output->position, data, size);
    output->position += size;
}

void mh_memory_write_string(mh_memory *output, const char *str) {
    mh_memory_write(output, str, strlen(str));
}
#endif //MHSERV_MH_MEMORY_H
