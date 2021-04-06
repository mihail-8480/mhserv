#ifndef MHSERV_MH_STREAM_H
#define MHSERV_MH_STREAM_H
#include <stdbool.h>
#include <stdlib.h>
#include "mh_memory.h"

// The memory stream structure
typedef struct mh_stream {
    // It should be empty
} mh_stream_t;

// Create a new memory stream
mh_stream_t* mh_memory_stream_new(size_t size, bool fixed);

// Get direct access to the memory stream's memory
mh_memory_t* mh_memory_stream_get_memory(mh_stream_t* stream);

// Create a new socket stream (will probably work with normal file descriptors too)
mh_stream_t* mh_socket_stream_new(int sock);

// Move the stream's position, if it returns false it means it failed
bool mh_stream_seek(mh_stream_t* stream, size_t position);

// Read from a stream to memory
void mh_stream_read(mh_stream_t* stream, mh_memory_t* buffer, size_t count);

// Write from memory to a stream
void mh_stream_write(mh_stream_t *stream, mh_memory_t* buffer, size_t count);

// Get the stream's position
size_t mh_stream_get_position(mh_stream_t *stream);

// Get the stream's size
size_t mh_stream_get_size(mh_stream_t *stream);

// Cleanup all the resources related to the stream
void mh_stream_free(mh_stream_t *stream);

// Copy bytes from one stream to an other
void mh_stream_copy_to(mh_stream_t* dest, mh_stream_t* src, size_t size);
#endif //MHSERV_MH_STREAM_H
