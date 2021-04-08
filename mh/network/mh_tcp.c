#include "mh_tcp.h"
#include "../mh_thread.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

// The new thread's arguments
typedef struct mh_tcp_threaded_args {
    int socket;
    mh_socket_address_t address;
    mh_on_connect_t on_connect;

} mh_tcp_threaded_args_t;

void* mh_tcp_threaded_connect_invoke(void* ptr) {
    // Get the arguments
    mh_tcp_threaded_args_t* args = ((mh_tcp_threaded_args_t*)ptr);

    mh_context_t* context = mh_start();
    // Call the on_connect method with the passed arguments
    args->on_connect(context, args->socket, args->address);

    // Free the previously allocated memory
    free(ptr);
    mh_end(context);

    return NULL;
}

bool mh_tcp_threaded = false;

void mh_tcp_threaded_set(bool use_threads) {
    mh_tcp_threaded = use_threads;
}

// For error reporting
mh_context_t *mh_tcp_last_context;

void mh_tcp_sigpipe(int sig) {
    mh_context_error(mh_tcp_last_context,"Broken pipe.", mh_tcp_sigpipe);
}

void mh_tcp_start(mh_context_t* context, const uint16_t port, const int max_clients, mh_on_connect_t on_connect) {
    // Handle broken pipes
    mh_tcp_last_context = context;
    signal(SIGPIPE, mh_tcp_sigpipe);

    // Create a new TCP socket
    int socket_fd;
    int opt = 1;
    // Set the address to * and the port to whatever the user specifies
    struct sockaddr_in address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };
    socklen_t addrLen = sizeof address;

    // If the socket isn't made, crash the program
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket");
        mh_context_error(context, "A socket could not be created successfully.", mh_tcp_start);
        abort();
    }

    // Set the socket options, if it fails, crash the program
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        mh_context_error(context, "Failed setting socket options.", mh_tcp_start);
        abort();
    }

    // Bind the socket to the address specified earlier
    if(bind(socket_fd, (struct sockaddr *)&address, addrLen) == -1) {
        perror("bind");
        mh_context_error(context, "Could not use the specified address.", mh_tcp_start);
        abort();
    }

    // Start listening
    int sock = listen(socket_fd, max_clients);
    if (sock == -1) {
        perror("listen");
        mh_context_error(context, "Failed listening for clients.", mh_tcp_start);
        abort();
    }

    // Forever... (until the program crashes)
    while(true) {
        // Accept a client
        int client = accept(socket_fd, (struct sockaddr *) &address, &addrLen);
        // If the client is invalid, crash the program
        if (client == -1) {
            perror("accept");
            mh_context_error(context, "Could not accept client.", mh_tcp_start);
            abort();
        }

        // If multi-threading is disabled
        if (!mh_tcp_threaded) {
            // Invoke the on_connect function directly
            mh_context_t* client_context = mh_start();
            on_connect(client_context, client, address);
            mh_end(client_context);
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
