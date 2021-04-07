#include "mh_stream.h"
#include "mh_stream_private.h"
#include "mh_memory.h"

// The memory stream, it's practically a dynamic array
typedef struct {
    mh_stream_private_t base;
    mh_memory_t* memory;
    bool fixed;
} mh_memory_stream_t;

void mh_memory_stream_read(void* self, mh_memory_t* buffer, size_t count) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    // Check if the memory that is being read is actually allocated
    if (this->memory->offset + count > this->memory->size) {
        // If not, report an error
        STREAM_ERROR("The memory you are trying to read is out of range.");
    }

    // Copy bytes and update the buffer offset
    memcpy(buffer->address, this->memory->address + this->memory->offset, count);
    buffer->offset = count;
    this->memory->offset += count;
}

static inline void mh_memory_stream_increase(mh_memory_stream_t* this, size_t minimal_size) {
    // Resize the memory to be at least minimal_size larger (or 2x, whatever is bigger)
    size_t increase = this->memory->offset * 2 > minimal_size ? this->memory->size * 2 : minimal_size;
    mh_memory_resize(this->memory, increase);
}

void mh_memory_stream_write(void* self, mh_memory_t* buffer, size_t count) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    // If the array is supposed to be fixed and there isn't enough space
    if (this->memory->offset + count > this->memory->size) {
        if (this->fixed) {
            // Report the error
            STREAM_ERROR("The memory you are trying to write is too large for this stream.");
        } else {
            // Allocate enough memory to complete the write operation
            mh_memory_stream_increase(this, this->memory->offset + count);
        }
    }
    // Copy bytes and update the buffer offset
    memcpy(this->memory->address + this->memory->offset, buffer->address, count);
    buffer->offset = count;
    this->memory->offset += count;
}


void mh_memory_stream_seek(void* self, size_t position) {
    // Set the memory offset
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    if (this->memory->offset + position < this->memory->size) {
        STREAM_ERROR("The position is larger than the memory size, cannot seek.");
    }

    this->memory->offset = position;
}

size_t mh_memory_stream_get_position(void *self) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    // Get the memory offset
    return this->memory->offset;
}
size_t mh_memory_stream_get_size(void* self) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    // Get the memory size
    return this->memory->size;
}

void mh_memory_stream_free(void* self) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    // Free the internal buffer
    mh_destructor_free(this->memory);
    // Free the resources used by the stream structure
    free(self);
}


mh_memory_t *mh_memory_stream_get_memory(mh_stream_t *stream) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)stream;

    // Get the memory address, offset and size
    return this->memory;
}

mh_stream_t *mh_memory_stream_new(size_t size, bool fixed) {
    mh_memory_stream_t* stream = malloc(sizeof(mh_memory_stream_t));
    stream->base.base.destructor.free = mh_memory_stream_free;

    // Override and enable reading
    stream->base.can_read = true;
    stream->base.read = mh_memory_stream_read;

    // Override and enable writing
    stream->base.can_write = true;
    stream->base.write = mh_memory_stream_write;

    // Override and enable seeking
    stream->base.can_seek = true;
    stream->base.seek = mh_memory_stream_seek;
    stream->base.get_position = mh_memory_stream_get_position;
    stream->base.get_size = mh_memory_stream_get_size;

    // Init the default values
    stream->memory = mh_memory_new(size, true);
    stream->fixed = fixed;
    return (mh_stream_t*)stream;
}