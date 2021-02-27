#include "mh_tcp.h"
#include "mh_http.h"

void mh_http_api(mh_memory* header, mh_memory* body, mh_http_request request) {
    printf("%s:%hu -> %s [%zu headers] `%s`\n", inet_ntoa(request.address.sin_addr),
           request.address.sin_port,request.top.type, request.headers->count, request.top.uri);
    HEADER("HTTP/1.1 200 OK\n");
    HEADER("Content-Type: text/html; charset=UTF-8\n");
    HEADER("Connection: close\n");
    ECHO("<h1>Hello, World!</h1>");
}
int main(void) {
    in_port_t port = 8080;
    printf("Starting server on port %hu...\n", port);
    mh_tcp(port, 50, mh_http_connect);
}
