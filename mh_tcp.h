#include "include.h"
#include "mh_error.h"

#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H

// A function pointer for the function that gets called when a client connects to the server
typedef void (*mh_on_connect)(int, struct sockaddr_in);

// Does nothing.
void do_nothing(int$){
    // Really.
}

// Create and start listening in TCP
_Noreturn void mh_tcp(const uint16_t port, const int max_clients, mh_on_connect onConnect) {
    // Ignore broken pipes
    signal(SIGPIPE, do_nothing);

    // Create a new TCP socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    // Set the address to * and the port to whatever the user specifies
    struct sockaddr_in address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };
    socklen_t addrlen = sizeof address;

    // If the socket isn't made, crash the program
    mh_error(socket_fd);

    // Set the socket options, if it fails, crash the program
    int setsockopt_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    mh_error(!setsockopt_result);

    // Bind the socket to the address specified earlier
    int bind_result = bind(socket_fd, (struct sockaddr *)&address, addrlen);
    mh_error(bind_result >= 0);

    // Start listening
    int sock = listen(socket_fd, max_clients);
    mh_error(sock >= 0);

    // Forever... (until the program crashes)
    while(true) {

        // Accept a client
        int client = accept(socket_fd, (struct sockaddr *) &address, &addrlen);
        // If the client is invalid, crash the program
        mh_error(client >= 0);
        // Call the onConnect event function
        onConnect(client, address);
    }

}
#endif //MHSERV_MH_TCP_H
