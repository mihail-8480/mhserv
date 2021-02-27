#include "include.h"
#include "mh_error.h"

#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H

typedef void (*mh_on_connect)(int, struct sockaddr_in);

void do_nothing(int signum){

}

_Noreturn void mh_tcp(const uint16_t port, const int max_clients, mh_on_connect onConnect) {
    signal(SIGPIPE, do_nothing);
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    struct sockaddr_in address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };
    socklen_t addrlen = sizeof address;
    mh_error(socket_fd);

    int setsockopt_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    mh_error(!setsockopt_result);

    int bind_result = bind(socket_fd, (struct sockaddr *)&address, addrlen);
    mh_error(bind_result >= 0);

    int sock = listen(socket_fd, max_clients);
    mh_error(sock >= 0);

    while(true) {
        int client = accept(socket_fd, (struct sockaddr *) &address, &addrlen);
        mh_error(client >= 0);
        onConnect(client, address);
    }

}
#endif //MHSERV_MH_TCP_H
