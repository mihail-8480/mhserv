#include "mh_stream.h"
#include "mh_stream_private.h"
#include "mh_memory.h"
#include <unistd.h>
#include <netdb.h>
// The socket stream structure
typedef struct {
    mh_stream_private_t base;
    int socket;
} mh_socket_stream_t;

void mh_socket_stream_read(void* self, mh_memory_t* buffer, size_t count) {
    mh_socket_stream_t* this = (mh_socket_stream_t*)self;

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

void mh_socket_stream_write(void* self, mh_memory_t* buffer, size_t count) {
    mh_socket_stream_t* this = (mh_socket_stream_t*)self;

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

void mh_socket_stream_free(void* self) {
    mh_socket_stream_t* this = (mh_socket_stream_t*)self;

    // Shutdown the socket
    shutdown(this->socket, SHUT_WR);
    // Close the socket
    close(this->socket);
    // Free the memory used by the stream
    free(self);
}

mh_stream_t *mh_socket_stream_new(mh_context_t* context, int socket) {
    mh_socket_stream_t* stream = malloc(sizeof(mh_socket_stream_t));
    stream->base.base.destructor.free = mh_socket_stream_free;
    stream->base.context = context;

    // Override and enable reading
    stream->base.can_read = true;
    stream->base.read = mh_socket_stream_read;

    // Override and enable writing
    stream->base.can_write = true;
    stream->base.write = mh_socket_stream_write;

    // Set to null and disable seeking
    stream->base.can_seek = false;
    stream->base.seek = NULL;
    stream->base.get_position = NULL;
    stream->base.get_size = NULL;
    
    stream->socket = socket;
    return (mh_stream_t*)stream;
}