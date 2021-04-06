#ifndef MHSERV_MH_STREAM_PRIVATE_H
#define MHSERV_MH_STREAM_PRIVATE_H
#include "mh_error.h"
#include "mh_memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Report a stream error
#define STREAM_ERROR(x) mh_error_report("Stream error: " # x)

// The private stream structure, it contains all the methods required for a stream implementation
typedef struct mh_stream_private {
    mh_stream_t base;
    void (*write)(void* self, mh_memory_t* buffer, size_t count);
    void (*read)(void* self, mh_memory_t* buffer, size_t count);
    void (*seek)(void* self, size_t position);
    void (*free)(void*self);
    size_t (*get_position)(void *self);
    size_t (*get_size)(void* self);
    bool can_read;
    bool can_write;
    bool can_seek;
} mh_stream_private_t;

#endif //MHSERV_MH_STREAM_PRIVATE_H
