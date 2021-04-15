#include "mh_tcp.h"
#include "../mh_thread.h"
#include <stdlib.h>
#include <sys/param.h>

#ifndef WIN32
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>

void mh_tcp_sigpipe(int sig) {
    // Try to get the context of the thread where the SIGPIPE happened
    mh_context_t* context = mh_context_get_from_thread();
    if (context == NULL) {
        return;
    }
    // Report the error on that context
    mh_context_error(context,"Broken pipe.", mh_tcp_sigpipe);
}
#else

#include <ws2tcpip.h>

#endif

// The new thread's arguments
typedef struct mh_tcp_threaded_args {
    mh_socket_t socket;
    mh_socket_address_t address;
    mh_on_connect_t on_connect;
    mh_context_t *context;

} mh_tcp_threaded_args_t;

void *mh_tcp_threaded_connect_invoke(void *ptr) {
    // Get the arguments
    MH_THIS(mh_tcp_threaded_args_t*, ptr);
    mh_context_bind_to_thread(this->context);

    // Call the on_connect method with the passed arguments
    this->on_connect(this->context, this->socket, this->address);

    // Free the previously allocated memory
    mh_end(this->context);
    return NULL;
}

void mh_tcp_start(mh_context_t *context, const mh_socket_address_t address, const int max_clients,
                  mh_on_connect_t on_connect) {

#ifndef WIN32
    // Handle broken pipes
    signal(SIGPIPE, mh_tcp_sigpipe);
    int opt = 1;
#else
    // Initiates Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        mh_context_error(context, "WSAStartup failed.", mh_tcp_start);
        abort();
    }
#endif

    mh_socket_t sock;

    socklen_t addr_len = sizeof(address);

    // If the socket isn't made, crash the program
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        mh_context_error(context, "A socket could not be created successfully.", mh_tcp_start);
        abort();
    }

#ifndef WIN32
#ifndef __FreeBSD__
    // Set the socket options, if it fails, crash the program
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        mh_context_error(context, "Failed setting socket options.", mh_tcp_start);
        abort();
    }
#endif
#endif

    // Bind the socket to the address specified earlier
    if (bind(sock, (struct sockaddr *) &address, addr_len) == -1) {
        mh_context_error(context, "Could not use the specified address.", mh_tcp_start);
        abort();
    }

    // Start listening
    if (listen(sock, max_clients) == -1) {
        mh_context_error(context, "Failed listening for clients.", mh_tcp_start);
        abort();
    }

    // Forever... (until the program crashes)
    while (true) {
        // Accept a client
        mh_socket_t client = accept(sock, (struct sockaddr *) &address, &addr_len);
        // If the client is invalid, crash the program
        if (client == -1) {
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

unsigned short mh_tcp_address_to_string(char *host, mh_socket_address_t address, size_t size) {
    inet_ntop(address.sin_family, &address.sin_addr, host, size);
    return ntohs(address.sin_port);
}

mh_socket_address_t mh_tcp_string_to_address(const char *host, unsigned short port) {
    mh_socket_address_t address = {
            .sin_family = AF_INET,
            .sin_port = htons(port)
    };
    inet_pton(address.sin_family, host, &address.sin_addr);
    return address;
}
