#define MH_MAIN
#include "mh/mh_console.h"
#include "mh/mh_tcp.h"
#include "mh/mh_http.h"
#include <arpa/inet.h>

void my_request_handler(mh_stream_t *socket_stream, mh_http_request_t *request) {

    printf("%s:%hu -> ", inet_ntoa(request->address.sin_addr),request->address.sin_port);
    mh_memory_print(request->method);
    printf(" [%zu headers] `", request->headers_count);
    mh_memory_print(request->url);
    printf("`\n");

    for (size_t i = 0; i < request->headers_count; i++) {
        printf("H:`");
        mh_memory_print(request->headers[i]);
        printf("`\n");
    }

    printf("Request content length: %zu\n", request->content.size);

    // Write some stuff to the server
    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);
    ECHO("<h1>Hello, World!</h1>");
}

int mh_main(mh_console_t console) {
    console.output.write("Hello, World!\n");
    console.output.write(console.argv[0]);
    // Set the request handler
    mh_http_set_request_handler(my_request_handler);
    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(8080, 32, mh_http);
}
