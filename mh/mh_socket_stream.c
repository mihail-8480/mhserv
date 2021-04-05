#include <unistd.h>
#include <netdb.h>
#include "mh_stream.h"
#include "mh_stream_private.h"
#include "mh_memory.h"

typedef struct {
    mh_stream_private_t base;
    int socket;
} mh_socket_stream_t;

void mh_socket_stream_read(void* self, mh_memory_t* buffer, size_t count) {
    mh_socket_stream_t* this = (mh_socket_stream_t*)self;
    ssize_t size = read(this->socket, buffer->address, count);
    if (size == -1) {
        STREAM_ERROR("Failed reading from the socket, it is probably closed.");
    }
    buffer->offset = size;
}

void mh_socket_stream_write(void* self, mh_memory_t* buffer, size_t count) {
    mh_socket_stream_t* this = (mh_socket_stream_t*)self;
    ssize_t size = write(this->socket, buffer->address, count);
    if (size == -1) {
        STREAM_ERROR("Failed writing to the socket, it is probably closed.");
    }
    buffer->offset = size;
}

void mh_socket_stream_free(void* self) {
    mh_socket_stream_t* this = (mh_socket_stream_t*)self;
    shutdown(this->socket, SHUT_WR);
    close(this->socket);
    free(self);
}

mh_stream_t *mh_socket_stream_new(int socket) {
    mh_socket_stream_t* stream = malloc(sizeof(mh_socket_stream_t));
    stream->base.free = mh_socket_stream_free;
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