#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H
#include <stdint.h>
#include <netinet/in.h>
#include <stdbool.h>

// A socket address (struct socketaddr_in) renamed, it's functionally the same
typedef struct sockaddr_in mh_socket_address_t;

// A function pointer type for mh_tcp_start
typedef void (*mh_on_connect_t)(int, mh_socket_address_t);

// Should we use multiple threads?
void mh_tcp_threaded_set(bool use_threads);

// Start a TCP listener
_Noreturn void mh_tcp_start(uint16_t port, int max_clients, mh_on_connect_t on_connect);
#endif //MHSERV_MH_TCP_H
