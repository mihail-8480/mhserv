#include "mh/network/mh_http.h"
#include "mh/mh_thread.h"
#include "mh/mh_handle.h"
#include <limits.h>

bool program_error(mh_context_t *context, const char *message, void *from) {
    fprintf(stderr, "[program_error] An error has occurred at function %#08zx: %s\n", (size_t) from, message);
    exit(1);
}

bool http_error(mh_context_t *context, const char *message, void *from) {
    fprintf(stderr, "[http_error] An error has occurred at function %#08zx: %s\n", (size_t) from, message);
    mh_end(context);
    mh_thread_exit(0);
}

inline static const char* mh_env_default(const char* env, const char* def) {
    const char *res = getenv(env);
    if (res != NULL) {
        return res;
    }
    return def;
}

int main(int argc, char **argv) {
    mh_context_t *context = mh_start();
    mh_context_set_error_handler(context, program_error);

    // Check if we should change the default port
    const char *e_port = mh_env_default("MH_PORT", "8080");
    unsigned long port = strtoul(e_port, NULL, 10);
    if (port >= USHRT_MAX) {
        mh_context_error(context, "The port number is too large.", main);
        return 1;
    }

    // Check if we should change the default library function
    const char *library_function = mh_env_default("MH_LIB_FUNCTION", "mh_http_handle");

    // Check if we should change the default IP
    const char *ip = mh_env_default("MH_IP", "127.0.0.1");

    // Check if the command line arguments are correct
    if (argc != 2) {
        mh_context_error(context, "Invalid syntax", main);
        return 1;
    }

    // Load the specified library and configure the http server
    mh_handle_t *library = mh_handle_new(context, argv[1]);
    mh_http_set_error_handler(http_error);
    mh_http_set_request_handler(mh_handle_find_symbol(library, library_function));

    // Convert the address to a string  (for verification purposes)
    mh_socket_address_t address = mh_tcp_string_to_address(ip, port);
    char adr_str[40];

    // Convert the string back to an address and print it (to see if everything is done fine)
    int adr_prt = mh_tcp_address_to_string(adr_str, address, 20);
    printf("Listening on http://%s:%d\n", adr_str, adr_prt);

    // Start the TCP listener
    mh_tcp_start(context, address, 32, mh_http);
}
