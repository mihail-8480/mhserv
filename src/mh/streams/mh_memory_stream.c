#include "mh_stream.h"
#include "mh_stream_private.h"

// The memory stream, it's practically a dynamic array
typedef struct mh_memory_stream {
    mh_stream_private_t base;
    mh_memory_t *memory;
    bool fixed;
} mh_memory_stream_t;

void mh_memory_stream_read(void *stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_memory_stream_t*, stream);
    // Check if the memory that is being read is actually allocated
    if (this->memory->offset + count > this->memory->size) {
        // If not, report an error
        mh_context_error(this->base.context, "The memory you are trying to read is out of range.",
                         mh_memory_stream_read);
        return;
    }

    // Copy bytes and update the buffer offset
    memcpy(buffer->address, this->memory->address + this->memory->offset, count);
    buffer->offset = count;
    this->memory->offset += count;
}

static inline void mh_memory_stream_increase(mh_memory_stream_t *this, size_t minimal_size) {
    // Resize the memory to be at least minimal_size larger (or 2x, whatever is bigger)
    size_t increase = this->memory->offset * 2 > minimal_size ? this->memory->size * 2 : minimal_size;
    mh_memory_resize(this->base.context, this->memory, increase);
}

void mh_memory_stream_write(void *stream, mh_memory_t *buffer, size_t count) {
    MH_THIS(mh_memory_stream_t*, stream);
    // If the array is supposed to be fixed and there isn't enough space
    if (this->memory->offset + count > this->memory->size) {
        if (this->fixed) {
            // Report the error
            mh_context_error(this->base.context, "The memory you are trying write is too large for this stream.",
                             mh_memory_stream_write);
            return;
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


void mh_memory_stream_seek(void *stream, size_t position) {
    // Set the memory offset
    MH_THIS(mh_memory_stream_t*, stream);
    if (this->memory->offset + position < this->memory->size) {
        mh_context_error(this->base.context, "The position is larger than the memory allocation_size, cannot seek.",
                         mh_memory_stream_seek);
        return;
    }

    this->memory->offset = position;
}

size_t mh_memory_stream_get_position(void *stream) {
    MH_THIS(mh_memory_stream_t*, stream);
    // Get the memory offset
    return this->memory->offset;
}

size_t mh_memory_stream_get_size(void *stream) {
    MH_THIS(mh_memory_stream_t*, stream);
    // Get the memory allocation_size
    return this->memory->size;
}


mh_memory_t *mh_memory_stream_get_memory(mh_stream_t *stream) {
    MH_THIS(mh_memory_stream_t*, stream);
    return this->memory;
}

mh_stream_t *mh_memory_stream_new(mh_context_t *context, size_t size, bool fixed) {
    MH_THIS(mh_memory_stream_t*, mh_context_allocate(context, sizeof(mh_memory_stream_t), false).ptr);

    this->base.base.destructor.free = NULL;
    this->base.context = context;
    mh_context_add_destructor(context, &this->base.base.destructor);

    // Override and enable reading
    this->base.can_read = true;
    this->base.read = mh_memory_stream_read;

    // Override and enable writing
    this->base.can_write = true;
    this->base.write = mh_memory_stream_write;

    // Override and enable seeking
    this->base.can_seek = true;
    this->base.seek = mh_memory_stream_seek;
    this->base.get_position = mh_memory_stream_get_position;
    this->base.get_size = mh_memory_stream_get_size;

    // Init the default values
    this->memory = mh_memory_new(context, size, true);
    this->fixed = fixed;
    return (mh_stream_t *) this;
}