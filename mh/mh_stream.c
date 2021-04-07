#include "mh_stream.h"
#include "mh_stream_private.h"
#include "mh_destructor.h"
bool mh_stream_seek(mh_stream_t *ptr, size_t position) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    if (stream->can_seek) {
        stream->seek(stream, position);
        return true;
    }
    return false;
}

void mh_stream_read(mh_stream_t *ptr, mh_memory_t* buffer, size_t count) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;

    if (stream->can_read) {
        stream->read(stream, buffer, count);
        return;
    }

    // Cannot read, report error
    STREAM_ERROR("Cannot read from this stream.");
}

void mh_stream_write(mh_stream_t *ptr, mh_memory_t* buffer, size_t count) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;

    if (stream->can_write) {
        stream->write(stream, buffer, count);
        return;
    }

    // Cannot write.
    STREAM_ERROR("Cannot write to the stream.");
}

size_t mh_stream_get_position(mh_stream_t *ptr) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    // Get the current position if it's possible
    if (stream->can_seek) {
        return stream->get_position(stream);
    }
    return -1;
}

size_t mh_stream_get_size(mh_stream_t *ptr) {
    mh_stream_private_t* stream = (mh_stream_private_t*)ptr;
    // Get the current size if it's possible
    if (stream->can_seek) {
        return stream->get_size(stream);
    }
    return -1;
}

void mh_stream_copy_to(mh_stream_t *dest, mh_stream_t *src, size_t size) {
    mh_stream_private_t* src_stream = (mh_stream_private_t*)src;
    mh_stream_private_t* dest_stream = (mh_stream_private_t*)dest;

    // If the streams are seekable, check if this operation is possible
    if (src_stream->can_seek && size > src_stream->get_size(src_stream)) {
        STREAM_ERROR("Not enough memory in src_stream to preform a mh_stream_copy_to operation.");
    }
    if (dest_stream->can_seek && size > dest_stream->get_size(dest_stream)) {
        STREAM_ERROR("Not enough memory in dest_stream to preform a mh_stream_copy_to operation.");
    }

    // try to NOT read everything at once...
    // or use an outside buffer
    mh_memory_t* buffer = mh_memory_new(size, false);
    mh_stream_read(src, buffer, size);
    mh_stream_write(dest, buffer, buffer->offset);
    mh_destructor_free(buffer);
}

size_t mh_stream_write_reference(mh_stream_t *stream, const void *ptr, size_t size) {
    // Create a memory reference
    mh_memory_t memory = mh_memory_reference((void*)ptr, size);
    // Write to the stream
    mh_stream_write(stream, &memory, memory.size);
    return memory.offset;
}
