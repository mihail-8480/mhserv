#include "mh/mh_tcp.h"
#include "mh/mh_http.h"

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
    generate_404(socket_stream, request);
}

int main(void) {
    // Set the request handler
    mh_http_set_request_handler(my_request_handler);
    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(8080, 32, mh_http);
}
