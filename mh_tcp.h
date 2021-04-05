#include "include.h"
#include "mh_error.h"
#include "mh_tasks.h"
#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H

// A function pointer for the function that gets called when a client connects to the server
typedef void (*mh_on_connect)(int, struct sockaddr_in);

// Does nothing.
void do_nothing(){
    // Really.
}

typedef struct {
    mh_on_connect onConnect;
    int client;
    struct sockaddr_in address;
} mh_con_task_args;

mh_task_result_t mh_tcp_connected_async(mh_task_args_t args) {
    mh_con_task_args* con_args = args;
    con_args->onConnect(con_args->client, con_args->address);
    free(args);
    return NULL;
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
    socklen_t addrLen = sizeof address;

    // If the socket isn't made, crash the program
    mh_error(socket_fd);

    // Set the socket options, if it fails, crash the program
    mh_error(!setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)));

    // Bind the socket to the address specified earlier
    mh_error(bind(socket_fd, (struct sockaddr *)&address, addrLen) >= 0);

    // Start listening
    int sock = listen(socket_fd, max_clients);
    mh_error(sock >= 0);

    // Forever... (until the program crashes)
    while(true) {
        // Accept a client
        int client = accept(socket_fd, (struct sockaddr *) &address, &addrLen);
        // If the client is invalid, crash the program
        mh_error(client >= 0);
        // Call the onConnect event function
#ifndef MH_ASYNC
        onConnect(client, address);
#else
        mh_con_task_args* args = malloc(sizeof(mh_con_task_args));
        args->onConnect = onConnect;
        args->address = address;
        args->client = client;
        mh_task task = mh_task_create(mh_tcp_connected_async, args, true);
        mh_task_run(task);
#endif
    }

}
#endif //MHSERV_MH_TCP_H
