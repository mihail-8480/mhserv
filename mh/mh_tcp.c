#include "mh_tcp.h"
#include "mh_error.h"
#include <signal.h>

void mh_tcp_start(const uint16_t port, const int max_clients, mh_on_connect on_connect) {
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
        on_connect(client, address);
    }
}
