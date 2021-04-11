#include "mh/network/mh_http.h"
#include "mh/mh_thread.h"
#include "mh/mh_handle.h"

bool program_error(mh_context_t* context, const char* message, void* from) {
    fprintf(stderr,"[program_error] An error has occurred at function %#08zx: %s\n", (size_t)from, message);
    exit(1);
}

bool http_error(mh_context_t* context, const char* message, void* from) {
    fprintf(stderr,"[http_error] An error has occurred at function %#08zx: %s\n", (size_t)from, message);
    mh_end(context);
    mh_thread_exit(0);
}

int main(int argc, char** argv) {
    mh_context_t* context = mh_start();
    mh_context_set_error_handler(context, program_error);

    // Check if the command line arguments are correct
    if (argc != 2) {
        mh_context_error(context, "Invalid syntax", main);
        return 1;
    }

    // Load the specified library and configure the http server
    mh_handle_t* library = mh_handle_new(context, argv[1]);
    mh_http_set_error_handler(http_error);
    mh_http_set_request_handler(mh_handle_find_symbol(library, MH_MACRO_STRINGIFY(MH_HTTP_HANDLE)));

    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(context,8080, 32, mh_http);
}
