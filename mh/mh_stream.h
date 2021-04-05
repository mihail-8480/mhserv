#ifndef MHSERV_MH_STREAM_H
#define MHSERV_MH_STREAM_H
#include <stdbool.h>
#include <stdlib.h>

// This should be empty
typedef struct mh_stream {
} mh_stream_t;

mh_stream_t* mh_memory_stream_new(size_t size, bool fixed);
bool mh_stream_seek(mh_stream_t* stream, size_t position);
char* mh_stream_read(mh_stream_t* stream, size_t count);
void mh_stream_write(mh_stream_t *stream, char* mem, size_t count);
size_t mh_stream_get_position(mh_stream_t *stream);
size_t mh_stream_get_size(mh_stream_t *stream);
void mh_stream_free(mh_stream_t *stream);
void mh_stream_copy_to(mh_stream_t* dest, mh_stream_t* src, size_t size);
#endif //MHSERV_MH_STREAM_H
