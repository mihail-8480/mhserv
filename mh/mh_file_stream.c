#include "mh_stream.h"
#include "mh_stream_private.h"
#include <unistd.h>
typedef struct {
    mh_stream_private_t base;
    FILE* file;
    bool should_close;
} mh_file_stream_t;

void mh_file_stream_read(void* self, mh_memory_t* buffer, size_t count) {
    mh_file_stream_t* this = (mh_file_stream_t*)self;

    // Read from the file
    size_t size = fread(buffer->address, 1, count, this->file);

    // If the size is negative, something went wrong
    if (size == -1) {
        STREAM_ERROR("Failed reading from the file, it is probably closed.");
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_file_stream_write(void* self, mh_memory_t* buffer, size_t count) {
    mh_file_stream_t* this = (mh_file_stream_t*)self;

    // Write to the file
    size_t size = fwrite(buffer->address, 1, count, this->file);

    // See above.
    if (size == -1) {
        STREAM_ERROR("Failed writing to the file, it is probably closed.");
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_file_stream_free(void* self) {
    mh_file_stream_t* this = (mh_file_stream_t*)self;
    // Close the file (if needed)
    if (this->should_close) {
        fclose(this->file);
    }
    // Free the memory used by the stream
    free(self);
}

void mh_file_stream_seek(void* self, size_t position) {
    mh_file_stream_t* this = (mh_file_stream_t*)self;

    fseek(this->file, position, SEEK_SET);
}
size_t mh_file_stream_get_position(void *self) {
    mh_file_stream_t* this = (mh_file_stream_t*)self;
    return ftell(this->file);
}

size_t mh_file_stream_get_size(void *self) {
    mh_file_stream_t* this = (mh_file_stream_t*)self;
    size_t old_position = ftell(this->file);
    fseek(this->file, 0, SEEK_END);
    size_t size = ftell(this->file);
    fseek(this->file, old_position, SEEK_SET);
    return size;
}

mh_stream_t *mh_file_stream_new(FILE* file, bool should_close) {
    mh_file_stream_t* stream = malloc(sizeof(mh_file_stream_t));
    stream->base.base.destructor.free = mh_file_stream_free;

    // Override and enable reading
    stream->base.can_read = true;
    stream->base.read = mh_file_stream_read;

    // Override and enable writing
    stream->base.can_write = true;
    stream->base.write = mh_file_stream_write;

    // Enable seeking
    stream->base.can_seek = true;
    stream->base.seek = mh_file_stream_seek;
    stream->base.get_position = mh_file_stream_get_position;
    stream->base.get_size = mh_file_stream_get_size;

    stream->file = file;
    stream->should_close = should_close;
    return (mh_stream_t*)stream;
}