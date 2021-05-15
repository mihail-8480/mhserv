#include <mh_http.h>
#include <mh_thread.h>
#include <mh_console.h>
#include <mh_handle.h>
#include <limits.h>

typedef void (*mh_lib_init_t)(mh_tcp_listener_t *listener);

bool http_error(mh_context_t *context, const char *message, mh_code_location_t from) {
    MH_WRITE_ERR("[HTTP] An error has occurred {}: {}\n", MH_FMT_LOC(&from), MH_FMT_STR(message));
    // End the context and exit the thread
    mh_end(context);
    mh_thread_exit(0);
}

mh_lib_init_t load_init(mh_handle_t *library, const char *library_init) {
    volatile mh_lib_init_t init = NULL;
    MH_WRITE("Attempting to load the initialization function `{}()`...", MH_FMT_STR(library_init));
    MH_TRY(init_try, MH_GLOBAL) {
        init = ((mh_lib_init_t)(size_t)mh_handle_find_symbol(library, library_init));
    }
    MH_CATCH(init_try, MH_GLOBAL, {
        MH_WRITE(" [FAILED]\n");
    });
    if (init != NULL) {
        MH_WRITE(" [OK]\n");
    }
    return init;
}

int main(int argc, char *argv[]) {
    // Check the libmh version
    mh_version_t version = mh_get_version();
    if (version.major == 0 && version.minor < 2) {
        MH_THROW(MH_GLOBAL, "Please use a version of libmh that is at least `0.2.0-alpha`!");
    }

    // Parse command line arguments
    mh_argument_parser_args_t args_conf = {
            .required_arguments = "lib",
            .optional_arguments = NULL
    };
    mh_map_t *args = mh_argument_parse(MH_GLOBAL, &args_conf, argc, argv);

    // Check if we should change the default port
    const char *e_port = mh_env_default("MH_PORT", "8080");
    unsigned long port = strtoul(e_port, NULL, 10);
    if (port >= USHRT_MAX) {
        MH_THROW(MH_GLOBAL, "The port number is too large.");
    }

    // Check if we should change the default library functions
    const char *library_function = mh_env_default("MH_LIB_FUNCTION", "mh_http_handle");
    const char *library_init = mh_env_default("MH_LIB_INIT", "mh_lib_init");

    // Check if we should change the default IP
    const char *ip = mh_env_default("MH_IP", "127.0.0.1");

    // Convert the address to a string  (for verification purposes)
    mh_socket_address_t address = mh_tcp_string_to_address(ip, port);
    char adr_str[40];

    // Convert the string back to an address and print it (to see if everything is done fine)
    int adr_prt = mh_tcp_address_to_string(adr_str, address, 20);

    // Load the specified module/library
    mh_handle_t *library = mh_handle_new(MH_GLOBAL, MH_MEM_TO_STRING(mh_map_get(args, MH_STRING("lib"))));

    // Configure the HTTP listener
    mh_tcp_listener_t *listener = mh_http_listener_new((mh_tcp_listener_t) {
            .context = MH_GLOBAL,
            .max_clients = 32,
            .address = address,
            .running = false
    });
    mh_http_set_error_handler(listener, http_error);
    mh_http_set_request_handler(listener,
                                (http_request_handler_t) (size_t) mh_handle_find_symbol(library, library_function));


    // Try to load the initialization function
    mh_lib_init_t init = load_init(library, library_init);
    // If the initialization function is loaded, execute it
    if (init != NULL) {
        init(listener);
    }

    // Start the TCP listener
    MH_WRITE("Listening on http://{}:{}...\n", MH_FMT_STR(adr_str), MH_FMT_UINT(adr_prt));
    mh_tcp_init(listener);
    mh_tcp_start(listener);

    // Cleanup
    mh_tcp_cleanup(listener);
}
