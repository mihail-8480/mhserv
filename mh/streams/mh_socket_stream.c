#include "mh_stream.h"
#include "mh_stream_private.h"
#include <unistd.h>
#include <netdb.h>
// The socket stream structure
typedef struct mh_socket_stream {
    mh_stream_private_t base;
    int socket;
} mh_socket_stream_t;

void mh_socket_stream_read(void* stream, mh_memory_t* buffer, size_t count) {
    MH_THIS(mh_socket_stream_t*, stream);

    // Read from the socket
    ssize_t size = read(this->socket, buffer->address, count);

    // If the allocation_size is negative, something went wrong
    if (size == -1) {
        mh_context_error(this->base.context,"Failed reading from the socket, it is probably closed.", mh_socket_stream_read);
        return;
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_socket_stream_write(void* stream, mh_memory_t* buffer, size_t count) {
    MH_THIS(mh_socket_stream_t*, stream);

    // Read from the socket
    ssize_t size = write(this->socket, buffer->address, count);

    // See above.
    if (size == -1) {
        mh_context_error(this->base.context,"Failed writing to the socket, it is probably closed.", mh_socket_stream_write);
        return;
    }

    // Change the buffer offset
    buffer->offset = size;
}

void mh_socket_stream_free(void* stream) {
    MH_THIS(mh_socket_stream_t*, stream);
    // Shutdown the socket
    shutdown(this->socket, SHUT_WR);
    // Close the socket
    close(this->socket);
}

mh_stream_t *mh_socket_stream_new(mh_context_t* context, int socket) {
    MH_THIS(mh_socket_stream_t*, mh_context_allocate(context, sizeof(mh_socket_stream_t), false).ptr);

    this->base.base.destructor.free = mh_socket_stream_free;
    this->base.context = context;
    mh_context_add_destructor(context, &this->base.base.destructor);

    // Override and enable reading
    this->base.can_read = true;
    this->base.read = mh_socket_stream_read;

    // Override and enable writing
    this->base.can_write = true;
    this->base.write = mh_socket_stream_write;

    // Set to null and disable seeking
    this->base.can_seek = false;
    this->base.seek = NULL;
    this->base.get_position = NULL;
    this->base.get_size = NULL;
    
    this->socket = socket;
    return (mh_stream_t*)this;
}