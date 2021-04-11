#include <unistd.h>
#include "mh/network/mh_http.h"
#include "mh/mh_thread.h"

bool tcp_error(mh_context_t* context, const char* message, void* from) {
    printf("%s\n", message);
    return true;
}

bool http_error(mh_context_t* context, const char* message, void* from) {
    mh_end(context);
    mh_thread_exit(0);
}

void generate_404(mh_stream_t *socket_stream, mh_http_request_t *request) {
    // 404 header
    ECHO("HTTP/1.1 404 Not Found" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);

    // 404 content
    ECHO("<h1>Not Found</h1>");
    ECHO("<p>The page you have requested at ");
    mh_stream_write(socket_stream, &request->url, request->url.size);
    ECHO(" was not found!</p>");
}

void send_file(mh_context_t* context, mh_stream_t *socket_stream, const char* file) {
    // text/plain header
    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/plain; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);

    // Send the file
    mh_stream_t* stream = mh_file_stream_new(context, fopen(file, "rb"), true);
    mh_stream_copy_to(socket_stream, stream, mh_stream_get_size(stream));
}

void my_request_handler(mh_context_t* context, mh_stream_t *socket_stream, mh_http_request_t *request) {

    char url[request->url.size+1];
    mh_memory_to_string(url, request->url);

    mh_memory_t host_mem = HEADER("Host");
    char host[host_mem.size+1];
    mh_memory_to_string(host, host_mem);

    printf("http://%s%s\n", host, url);

    /*
     * This is how you would read the rest of the POST content
     * if (memcmp(request->method.address, "POST", 4) == 0) {
     *  mh_http_request_read_content(socket_stream, request);
     * }
     */

    // If the file exists, send it, if not, send a 404
    if(access(url, R_OK) == 0) {
        send_file(context, socket_stream, url);
    } else {
        generate_404(socket_stream, request);
    }
}


int main(void) {
    // Configure
    mh_context_t* context = mh_start();
    mh_context_set_error_handler(context, tcp_error);
    mh_http_set_request_handler(my_request_handler);
    mh_http_set_error_handler(http_error);

    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(context,8080, 32, mh_http);
}
