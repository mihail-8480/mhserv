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
    mh_context_t* context;

} mh_tcp_threaded_args_t;

void* mh_tcp_threaded_connect_invoke(void* ptr) {
    // Get the arguments
    MH_THIS(mh_tcp_threaded_args_t*, ptr);
    mh_context_bind_to_thread(this->context);

    // Call the on_connect method with the passed arguments
    this->on_connect(this->context, this->socket, this->address);

    // Free the previously allocated memory
    mh_end(this->context);
    return NULL;
}

void mh_tcp_sigpipe(int sig) {
    // Try to get the context of the thread where the SIGPIPE happened
    mh_context_t* context = mh_context_get_from_thread();
    if (context == NULL) {
        return;
    }
    // Report the error on that context
    mh_context_error(context,"Broken pipe.", mh_tcp_sigpipe);
}

void mh_tcp_start(mh_context_t* context, const uint16_t port, const int max_clients, mh_on_connect_t on_connect) {
    // Handle broken pipes
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

        mh_context_t *client_context = mh_start();

        // Allocate the arguments that get passed to the new thread
        mh_tcp_threaded_args_t *args = mh_context_allocate(client_context, sizeof(mh_tcp_threaded_args_t), true).ptr;

        // Copy the needed arguments
        args->address = address;
        args->socket = client;
        args->on_connect = on_connect;
        args->context = client_context;

        // Create the thread with those arguments
        mh_thread_create(mh_tcp_threaded_connect_invoke, args);
    }
}
