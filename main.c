#include <stdio.h>
#include "mh/mh_tcp.h"
#include "mh/mh_stream.h"
void on_connect(int socket, mh_socket_address address) {
    mh_stream_t* socket_stream = mh_socket_stream_new(socket);
    mh_stream_t* request = mh_memory_stream_new(1024, false);
    mh_stream_copy_to(request, socket_stream, 1024);
    mh_stream_seek(request, 0);
    mh_memory* request_memory = mh_memory_stream_get_memory(request);
    printf("%s\n", (char*)request_memory->address);
    mh_stream_free(socket_stream);
    mh_stream_free(request);
}
int main(void) {
    mh_tcp_start(8080, 32, on_connect);
}
