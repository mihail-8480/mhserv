#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H
#include <stdint.h>
#include <netinet/in.h>

// A socket address (struct socketaddr_in) renamed, it's functionally the same
typedef struct sockaddr_in mh_socket_address;

// A function pointer type for mh_tcp_start
typedef void (*mh_on_connect)(int, mh_socket_address);

// Start a TCP listener
_Noreturn void mh_tcp_start(uint16_t port, int max_clients, mh_on_connect on_connect);
#endif //MHSERV_MH_TCP_H
