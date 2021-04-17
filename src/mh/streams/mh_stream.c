#include "mh_stream.h"
#include "mh_stream_private.h"

bool mh_stream_seek(mh_stream_t *stream, size_t position) {
    MH_THIS(mh_stream_private_t*, stream);
    if (this->can_seek) {
        this->seek(this, position);
        return true;
    }
    return false;
}

void mh_stream_read(mh_stream_t *stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_stream_private_t*, stream);

    if (this->can_read) {
        this->read(this, buffer, count);
        return;
    }

    // Cannot read, report error
    mh_context_error(this->context, "Cannot read from this stream.", mh_stream_read);
}

void mh_stream_write(mh_stream_t *stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_stream_private_t*, stream);
    if (this->can_write) {
        this->write(this, buffer, count);
        return;
    }

    // Cannot write.
    mh_context_error(this->context, "Cannot write to this stream.", mh_stream_read);
}

size_t mh_stream_get_position(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    // Get the current position if it's possible
    if (this->can_seek) {
        return this->get_position(this);
    }
    return -1;
}

size_t mh_stream_get_size(mh_stream_t *stream) {
    MH_THIS(mh_stream_private_t*, stream);
    // Get the current allocation_size if it's possible
    if (this->can_seek) {
        return this->get_size(this);
    }
    return -1;
}

void mh_stream_copy_to(mh_stream_t *dest, mh_stream_t *src, size_t size) {
    mh_stream_private_t *src_stream = (mh_stream_private_t *) src;
    mh_stream_private_t *dest_stream = (mh_stream_private_t *) dest;

    // If the streams are seekable, check if this operation is possible
    if (src_stream->can_seek && size > src_stream->get_size(src_stream)) {
        mh_context_error(src_stream->context,
                         "Not enough memory in src_stream to preform a mh_stream_copy_to operation.",
                         mh_stream_copy_to);
        return;
    }
    if (dest_stream->can_seek && size > dest_stream->get_size(dest_stream)) {
        mh_context_error(dest_stream->context,
                         "Not enough memory in dest_stream to preform a mh_stream_copy_to operation.",
                         mh_stream_copy_to);
        return;
    }

    // try to NOT read everything at once...
    // or use an outside buffer
    mh_memory_t *buffer = mh_memory_new(dest_stream->context, size, false);
    mh_stream_read(src, buffer, size);
    mh_stream_write(dest, buffer, buffer->offset);
}

size_t mh_stream_write_reference(mh_stream_t *stream, const void *ptr, size_t size) {
    // Create a memory reference
    mh_memory_t memory = mh_memory_reference((void *) ptr, size);
    // Write to the stream
    mh_stream_write(stream, &memory, memory.size);
    return memory.offset;
}
