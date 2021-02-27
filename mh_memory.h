#include "include.h"
#include "mh_buffer.h"

#ifndef MHSERV_MH_MEMORY_H
#define MHSERV_MH_MEMORY_H

// A buffer with a position, inspired by C#'s MemoryStream
typedef struct {
    mh_buffer buffer;
    size_t position;
} mh_memory;

// Create a new memory stream
mh_memory mh_memory_new(int size) {
    // Create a buffer
    mh_buffer buffer = mh_buffer_new(size);
    // Set the position to 0
    size_t position = 0;
    // Create a struct based on those
    return (mh_memory){buffer, position};
}

// Write bytes to the memory stream
void mh_memory_write(mh_memory *output, const void* data, size_t size) {
    // If it's too small, expand it to be big enough to read the data
    while (size + output->position >= output->buffer.size) {

        // By doubling the buffer
        mh_buffer_double(&output->buffer);
    }

    // Copy the bytes
    memcpy(output->buffer.ptr + output->position, data, size);

    // Move the position
    output->position += size;
}

// Write a string to the memory stream
void mh_memory_write_string(mh_memory *output, const char *str) {
    mh_memory_write(output, str, strlen(str));
}
#endif //MHSERV_MH_MEMORY_H
