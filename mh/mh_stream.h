#ifndef MHSERV_MH_STREAM_H
#define MHSERV_MH_STREAM_H
#include "mh_memory.h"
#include "mh_destructor.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// The memory stream structure
typedef struct mh_stream {
    mh_destructor_t destructor;
    // It should be empty
} mh_stream_t;

// Create a new memory stream
mh_stream_t* mh_memory_stream_new(size_t size, bool fixed);

// Get direct access to the memory stream's memory
mh_memory_t* mh_memory_stream_get_memory(mh_stream_t* stream);

// Create a new socket stream (will probably work with normal file descriptors too)
mh_stream_t* mh_socket_stream_new(int sock);

// Create a new file stream
mh_stream_t *mh_file_stream_new(FILE* file, bool should_close);

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

// Copy bytes from one stream to an other
void mh_stream_copy_to(mh_stream_t* dest, mh_stream_t* src, size_t size);

// Write a string to a stream without copying it, return how many bytes were actually written
size_t mh_stream_write_reference(mh_stream_t* stream, const void* ptr, size_t size);

#endif //MHSERV_MH_STREAM_H
