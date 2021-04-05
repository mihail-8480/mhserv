#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H
#include <stdint.h>
#include <netinet/in.h>
typedef struct sockaddr_in mh_socket_address;
typedef void (*mh_on_connect)(int, mh_socket_address);
_Noreturn void mh_tcp_start(uint16_t port, int max_clients, mh_on_connect onConnect);
#endif //MHSERV_MH_TCP_H
