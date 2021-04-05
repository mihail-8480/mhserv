#define MH_ASYNC

#include "mh_tcp.h"
#include "mh_http.h"

// Do some HTTP stuff
void mh_http_api(mh_memory* header, mh_memory* body, mh_request* request) {

    // Print some useful info about the client
    printf("%s:%hu -> %s [%zu headers] `%s`\n", inet_ntoa(request->address.sin_addr),
           request->address.sin_port,request->method, request->header_count, request->url);
    for (size_t i = 0; i < request->header_count; i++)
    {
        printf("H:`%s`\n",request->headers[i]);
    }

    // Send the status info
    HEADER("HTTP/1.1 200 OK");

    // Send the content type
    HEADER("Content-Type: text/html; charset=UTF-8");

    // Close the connection
    HEADER("Connection: close");

    // Best website of 2021
    ECHO("<h1>Hello, World!</h1>");
#ifdef MH_ASYNC
    ECHO("<p>Server is running in async mode.</p>");
#else
    ECHO("<p>Server is running in single-thread mode.</p>");
#endif
}
int main(void) {
    // TODO: add command line arguments
    in_port_t port = 8080;

    // Start http server on 8080
    printf("Starting server on port %hu...\n", port);
    mh_tcp(port, 50, mh_http_connect);

}
