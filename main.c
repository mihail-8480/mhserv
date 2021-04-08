#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mh/network/mh_http.h"
#include "mh/streams/mh_console.h"

bool tcp_error(mh_context_t* context, const char* message, void* from) {
    if (strcmp(message, "Broken pipe.") == 0) {
        mh_console.error.write(context, "Broken pipe, possible memory leak.\n");
        return true;
    }
    mh_console.error.write(context, message);
    mh_console.error.write(context, "\n");
    return false;
}

bool http_error(mh_context_t* context, const char* message, void* from) {
    mh_console.error.write(context, message);
    mh_console.error.write(context, "\n");
    mh_end(context);
    pthread_exit(0);
}

void generate_404(mh_stream_t *socket_stream, mh_http_request_t *request) {
    ECHO("HTTP/1.1 404 Not Found" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);

    ECHO("<h1>Not Found</h1>");
    ECHO("<p>The page you have requested at ");
    mh_stream_write(socket_stream, &request->url, request->url.size);
    ECHO(" was not found!</p>");
}

void send_file(mh_context_t* context, mh_stream_t *socket_stream, const char* file) {
    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/plain; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);

    mh_stream_t* stream = mh_file_stream_new(context, fopen(file, "rb"), true);
    mh_context_add_destructor(context, &stream->destructor);
    mh_stream_copy_to(socket_stream, stream, mh_stream_get_size(stream));
}

void my_request_handler(mh_context_t* context, mh_stream_t *socket_stream, mh_http_request_t *request) {
    // TODO: Add a hashmap for headers

    char url[request->url.size+1];
    memcpy(url, request->url.address, request->url.size);
    url[request->url.size] = 0;

    if(access(url, R_OK) == 0) {
        send_file(context, socket_stream, url);
    } else {
        generate_404(socket_stream, request);
    }


}

int main(void) {
    mh_context_t* context = mh_start();

    // Configure
    mh_tcp_threaded_set(true);
    mh_http_set_request_handler(my_request_handler);
    mh_http_set_error_handler(http_error);
    mh_context_set_error_handler(context, tcp_error);

    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(context,8080, 32, mh_http);
}
