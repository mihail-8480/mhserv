#include <dlfcn.h>
#include "mh/network/mh_http.h"
#include "mh/mh_thread.h"

bool tcp_error(mh_context_t* context, const char* message, void* from) {
    fprintf(stderr,"[tcp_error] An error has occurred at function %#08zx: %s\n", (size_t)from, message);
    return false;
}

bool http_error(mh_context_t* context, const char* message, void* from) {
    fprintf(stderr,"[http_error] An error has occurred at function %#08zx: %s\n", (size_t)from, message);
    mh_end(context);
    mh_thread_exit(0);
}

const char loader_error_default[] = "No error was reported.";
const char *loader_error = loader_error_default;

void report_error(mh_stream_t *socket_stream, mh_http_request_t *request) {
    ECHO("HTTP/1.1 500 Internal Server Error" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);
    ECHO("<h1>Internal Server Error</h1>");
    ECHO("<p>The server has reported: <code>");
    ECHO_STR(loader_error);
    ECHO("</code></p>");
}

void (*loaded_request)(mh_context_t*, mh_stream_t*, mh_http_request_t*) = NULL;

void dynamic_request(mh_context_t* context, mh_stream_t *socket_stream, mh_http_request_t *request) {
    if (loaded_request == NULL) {
        report_error(socket_stream, request);
        return;
    }
    loaded_request(context, socket_stream, request);
}


void* load(int argc, char** argv) {
    void *handle;
    if (argc != 2) {
        loader_error = "No request handler has been loaded because the argument count did not equal one.";
        return NULL;
    }
    handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        loader_error = dlerror();
        return NULL;
    }
    void* func = dlsym(handle, "http_handle");
    if (!func) {
        loader_error = dlerror();
        dlclose(handle);
        return NULL;
    }
    loaded_request = func;
    return handle;
}

int main(int argc, char** argv) {
    // Configure
    mh_context_t* context = mh_start();
    mh_context_set_error_handler(context, tcp_error);
    mh_http_set_request_handler(dynamic_request);
    mh_http_set_error_handler(http_error);

    load(argc, argv);

    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(context,8080, 32, mh_http);
}
