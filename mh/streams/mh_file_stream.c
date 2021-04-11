#include "mh_stream.h"
#include "mh_stream_private.h"
#include <unistd.h>

// The file stream structure
typedef struct mh_file_stream {
    mh_stream_private_t base;
    FILE* file;
    bool should_close;
} mh_file_stream_t;

void mh_file_stream_read(void* stream, mh_memory_t* buffer, size_t count) {
    MH_THIS(mh_file_stream_t*, stream);

    // Read from the file
    size_t size = fread(buffer->address, 1, count, this->file);

    // If the allocation_size is negative, something went wrong
    if (size == -1) {
        mh_context_error(this->base.context, "Failed reading from the file, it is probably closed.", mh_file_stream_read);
        return;
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_file_stream_write(void* stream, mh_memory_t* buffer, size_t count) {
    MH_THIS(mh_file_stream_t*, stream);

    // Write to the file
    size_t size = fwrite(buffer->address, 1, count, this->file);

    // See above.
    if (size == -1) {
        mh_context_error(this->base.context, "Failed writing to the file, it is probably closed.",mh_file_stream_write);
        return;
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_file_stream_free(void* stream) {
    MH_THIS(mh_file_stream_t*, stream);
    // Close the file (if needed)
    if (this->should_close) {
        fclose(this->file);
    }
}

void mh_file_stream_seek(void* stream, size_t position) {
    MH_THIS(mh_file_stream_t*, stream);
    fseeko(this->file, position, SEEK_SET);
}
size_t mh_file_stream_get_position(void *stream) {
    MH_THIS(mh_file_stream_t*, stream);
    return ftello(this->file);
}

size_t mh_file_stream_get_size(void *stream) {
    MH_THIS(mh_file_stream_t*, stream);
    // Save the old position
    size_t old_position = ftello(this->file);

    // Seek to the end
    fseeko(this->file, 0, SEEK_END);

    // Get the position (of the end)
    size_t size = ftello(this->file);

    // Seek back to the old position
    fseeko(this->file, old_position, SEEK_SET);
    return size;
}

mh_stream_t *mh_file_stream_new(mh_context_t* context, FILE* file, bool should_close) {
    MH_THIS(mh_file_stream_t*, mh_context_allocate(context, sizeof(mh_file_stream_t), false).ptr);
    this->base.base.destructor.free = mh_file_stream_free;
    this->base.context = context;
    mh_context_add_destructor(context, &this->base.base.destructor);

    // Override and enable reading
    this->base.can_read = true;
    this->base.read = mh_file_stream_read;

    // Override and enable writing
    this->base.can_write = true;
    this->base.write = mh_file_stream_write;

    // Enable seeking
    this->base.can_seek = true;
    this->base.seek = mh_file_stream_seek;
    this->base.get_position = mh_file_stream_get_position;
    this->base.get_size = mh_file_stream_get_size;

    this->file = file;
    this->should_close = should_close;
    return (mh_stream_t*)this;
}