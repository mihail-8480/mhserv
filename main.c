#include "mh/mh_tcp.h"
#include "mh/mh_http.h"

#include <arpa/inet.h>
#include <stdio.h>


void my_request_handler(mh_stream_t *socket_stream, mh_http_request_t *request) {
    // Print the request information
    printf("%s:%hu -> %s [%zu headers] `%s`\n", inet_ntoa(request->address.sin_addr),
           request->address.sin_port,request->method, request->headers_count, request->url);
    for (size_t i = 0; i < request->headers_count; i++)
        printf("H:`%s`\n",request->headers[i]);

    printf("Request content length: %zu\n", request->content.size);

    // Write some stuff to the server
    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);
    ECHO("<h1>Hello, World!</h1>");
}



int main(void) {
    // Set the request handler
    mh_http_set_request_handler(my_request_handler);
    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(8080, 32, mh_http);
}
