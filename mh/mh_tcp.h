#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H
#include <stdint.h>
#include <netinet/in.h>

typedef void (*mh_on_connect)(int, struct sockaddr_in);
_Noreturn void mh_tcp_start(uint16_t port, int max_clients, mh_on_connect onConnect);
#endif //MHSERV_MH_TCP_H
