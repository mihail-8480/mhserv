#include <stdio.h>
#include "mh/mh_tcp.h"
#include "mh/mh_stream.h"
const size_t copy_buffer_size = 128;
const size_t max_request_size = 1024;

void on_connect(int socket, mh_socket_address address) {
    // Create the streams
    mh_stream_t* socket_stream = mh_socket_stream_new(socket);
    mh_stream_t* request = mh_memory_stream_new(copy_buffer_size, false);

    // Read the whole request
    do {
        mh_stream_copy_to(request, socket_stream, copy_buffer_size);
        if (mh_stream_get_size(request) >= max_request_size) {
            break;
        }
    } while(mh_stream_get_position(request) == mh_stream_get_size(request));


    mh_memory* request_memory = mh_memory_stream_get_memory(request);
    printf("%zu %s\n", mh_stream_get_position(request), (char*)request_memory->address);

    // Free the memory
    mh_stream_free(socket_stream);
    mh_stream_free(request);
}
int main(void) {
    mh_tcp_start(8080, 32, on_connect);
}
