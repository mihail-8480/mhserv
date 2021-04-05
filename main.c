#include <string.h>
#include "mh/mh_tcp.h"
#include "mh/mh_stream.h"
const size_t copy_buffer_size = 128;
const size_t max_request_size = 1024;

// Why allocate stuff when you don't have to
#define ECHO(str) write_string(socket_stream, str, sizeof(str)-1)
static inline void write_string(mh_stream_t* stream, char* str, size_t len) {
    mh_memory_t memory = mh_memory_reference(str, len);
    mh_stream_write(stream, &memory, memory.size);
}

void on_connect(int socket, mh_socket_address address) {
    // Create the streams
    mh_stream_t* socket_stream = mh_socket_stream_new(socket);
    mh_stream_t* request = mh_memory_stream_new(copy_buffer_size, false);

    // Read the whole request, in the future read until \r\n\r\n
    // and put the excess stuff in an other request stream (for POST requests)
    // and make the max_request_size only apply to the header
    do {
        mh_stream_copy_to(request, socket_stream, copy_buffer_size);
        if (mh_stream_get_size(request) >= max_request_size) {
            break;
        }
    } while(mh_stream_get_position(request) == mh_stream_get_size(request));

    // Write some stuff to the server
    ECHO("HTTP/1.1 200 OK\r\n");
    ECHO("Content-Type: text/html; charset=UTF-8\r\n");
    ECHO("Connection: close\r\n");
    ECHO("\r\n");
    ECHO("<h1>Hello, World!</h1>");

    // Free the memory
    mh_stream_free(socket_stream);
    mh_stream_free(request);
}
int main(void) {
    mh_tcp_start(8080, 32, on_connect);
}
