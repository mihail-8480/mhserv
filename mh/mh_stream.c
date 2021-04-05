#include "mh_stream.h"
#include "mh_stream_private.h"
bool mh_stream_seek(mh_stream_t *ptr, size_t position) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    if (stream->can_seek && position < stream->get_size(stream) + 1) {
        stream->seek(stream, position);
        return true;
    }
    return false;
}

char *mh_stream_read(mh_stream_t *ptr, size_t count) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    if (stream->can_read) {
        char *result = stream->read(stream, count);
        if (stream->can_seek && !mh_stream_seek(ptr, stream->get_position(stream) + count)) {
            STREAM_ERROR("Failed seeking because you are trying to seek to a position that is out of range.");
        }
        return result;
    }
    STREAM_ERROR("Failed reading from the stream.");
    return NULL;
}

void mh_stream_write(mh_stream_t *ptr, char* mem, size_t count) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    if (stream->can_write) {
        stream->write(stream, mem, count);
        if (stream->can_seek && !mh_stream_seek(ptr, stream->get_position(stream) + count)) {
            STREAM_ERROR("Failed seeking because you are trying to seek to a position that is out of range.");
        }
        return;
    }
    STREAM_ERROR("Failed writing to the stream.");
}

size_t mh_stream_get_position(mh_stream_t *ptr) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    if (stream->can_seek) {
        return stream->get_position(stream);
    }
    return -1;
}

size_t mh_stream_get_size(mh_stream_t *ptr) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    if (stream->can_seek) {
        return stream->get_size(stream);
    }
    return -1;
}

void mh_stream_free(mh_stream_t *ptr) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    stream->free(stream);
}

void mh_stream_copy_to(mh_stream_t *dest, mh_stream_t *src, size_t size) {
    mh_stream_private_t* src_stream = (mh_stream_private_t*)src;
    mh_stream_private_t* dest_stream = (mh_stream_private_t*)dest;
    if (src_stream->can_seek && size > src_stream->get_size(src_stream)) {
        STREAM_ERROR("Not enough memory in src_stream to preform a mh_stream_copy_to operation.");
    }
    if (dest_stream->can_seek && size > dest_stream->get_size(dest_stream)) {
        STREAM_ERROR("Not enough memory in dest_stream to preform a mh_stream_copy_to operation.");
    }
    char *buff = mh_stream_read(src, size);
    mh_stream_write(dest, buff, size);
    free(buff);
}
