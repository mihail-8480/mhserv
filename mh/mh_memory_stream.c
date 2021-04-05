#include "mh_stream.h"
#include "mh_stream_private.h"
#include "mh_memory.h"

typedef struct {
    mh_stream_private_t base;
    mh_memory* memory;
    bool fixed;
} mh_memory_stream_t;

void mh_memory_stream_read(void* self, mh_memory* buffer, size_t count) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    if (this->memory->offset + count > this->memory->size) {
        STREAM_ERROR("The memory you are trying to read is out of range.");
    }
    memcpy(buffer->address, this->memory->address + this->memory->offset, count);
    buffer->offset = count;
}

void mh_memory_stream_increase(mh_memory_stream_t* this, size_t minimal_size) {
    size_t increase = this->memory->offset * 2 > minimal_size ? this->memory->size * 2 : minimal_size;
    mh_memory_resize(this->memory, increase);
}

void mh_memory_stream_write(void* self, mh_memory* buffer, size_t count) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    if (this->memory->offset + count > this->memory->size) {
        if (this->fixed) {
            STREAM_ERROR("The memory you are trying to write is too large for this stream.");
        } else {
            mh_memory_stream_increase(this, this->memory->offset + count);
        }
    }
    memcpy(this->memory->address + this->memory->offset, buffer->address, count);
    buffer->offset = count;
}

void mh_memory_stream_seek(void* self, size_t position) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    this->memory->offset = position;
}

size_t mh_memory_stream_get_position(void *self) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    return this->memory->offset;
}
size_t mh_memory_stream_get_size(void* self) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    return this->memory->size;
}

void mh_memory_stream_free(void* self) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)self;
    mh_memory_free(this->memory);
    free(self);
}


mh_memory *mh_memory_stream_get_memory(mh_stream_t *stream) {
    mh_memory_stream_t* this = (mh_memory_stream_t*)stream;
    return this->memory;
}

mh_stream_t *mh_memory_stream_new(size_t size, bool fixed) {
    mh_memory_stream_t* stream = malloc(sizeof(mh_memory_stream_t));
    stream->base.free = mh_memory_stream_free;
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