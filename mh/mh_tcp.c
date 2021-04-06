#include "mh_tcp.h"
#include "mh_error.h"
#include "mh_thread.h"
#include <signal.h>
#include <stdlib.h>

// The new thread's arguments
typedef struct mh_tcp_threaded_args {
    int socket;
    mh_socket_address_t address;
    mh_on_connect_t on_connect;

} mh_tcp_threaded_args_t;

void* mh_tcp_threaded_connect_invoke(void* ptr) {
    // Get the arguments
    mh_tcp_threaded_args_t* args = ((mh_tcp_threaded_args_t*)ptr);

    // Call the on_connect method with the passed arguments
    args->on_connect(args->socket, args->address);

    // Free the previously allocated memory
    free(ptr);
    return NULL;
}

bool mh_tcp_threaded = false;

void mh_tcp_threaded_set(bool use_threads) {
    mh_tcp_threaded = use_threads;
}

void mh_tcp_start(const uint16_t port, const int max_clients, mh_on_connect_t on_connect) {
    // Ignore broken pipes
    signal(SIGPIPE, SIG_IGN);

    // Create a new TCP socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    // Set the address to * and the port to whatever the user specifies
    struct sockaddr_in address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };
    socklen_t addrLen = sizeof address;

    // If the socket isn't made, crash the program
    mh_error_report_internal(socket_fd);

    // Set the socket options, if it fails, crash the program
    mh_error_report_internal(!setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)));

    // Bind the socket to the address specified earlier
    mh_error_report_internal(bind(socket_fd, (struct sockaddr *)&address, addrLen) >= 0);

    // Start listening
    int sock = listen(socket_fd, max_clients);
    mh_error_report_internal(sock >= 0);

    // Forever... (until the program crashes)
    while(true) {
        // Accept a client
        int client = accept(socket_fd, (struct sockaddr *) &address, &addrLen);
        // If the client is invalid, crash the program
        mh_error_report_internal(client >= 0);

        // If multi-threading is disabled
        if (!mh_tcp_threaded) {
            // Invoke the on_connect function directly
            on_connect(client, address);
        } else {
            // If multi-threading is enabled, allocate the arguments that get passed to the new list
            mh_tcp_threaded_args_t* args = calloc(1,sizeof(mh_tcp_threaded_args_t));
            // Copy the needed arguments
            args->address = address;
            args->socket = client;
            args->on_connect = on_connect;

            // Create the thread with those arguments
            mh_thread_create(mh_tcp_threaded_connect_invoke, args);
        }
    }
}
