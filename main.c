#include "mh/mh_tcp.h"
#include "mh/mh_http.h"
#include "mh/mh_console.h"

void generate_404(mh_stream_t *socket_stream, mh_http_request_t *request) {
    // Send the headers
    ECHO("HTTP/1.1 404 Not Found" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);

    // Send the content
    ECHO("<h1>Not Found</h1>");
    ECHO("<p>The page you have requested at ");
    mh_stream_write(socket_stream, &request->url, request->url.size);
    ECHO(" was not found!</p>");
}

void my_request_handler(mh_stream_t *socket_stream, mh_http_request_t *request) {
    // Currently it only sends a 404
    // TODO: Add a hashmap for headers
    // TODO: Make all the user exceptions free the used memory and recover

    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/plain; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);

    // Fix file streams
    mh_stream_t* stream = mh_file_stream_new(fopen("/etc/hosts", "rb"), true);
    size_t size = mh_stream_get_size(stream);
    size_t position = mh_stream_get_position(stream);
    printf("file = %zu/%zu\n", position, size);
    mh_memory_t* buffer = mh_memory_new(size, false);
    mh_stream_read(stream, buffer, size);
    size = mh_stream_get_size(stream);
    position = mh_stream_get_position(stream);
    printf("file = %zu/%zu\n", position, size);

    mh_stream_write(socket_stream, buffer, buffer->offset);

    mh_destructor_free(stream);
    mh_destructor_free(buffer);

    //generate_404(socket_stream, request);
}

int main(void) {
    mh_tcp_threaded_set(true);
    // Set the request handler
    mh_http_set_request_handler(my_request_handler);
    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(8080, 32, mh_http);
}
