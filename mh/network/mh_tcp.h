#ifndef MHSERV_MH_TCP_H
#define MHSERV_MH_TCP_H

#include "../mh_context.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#else

#include <winsock2.h>

#endif

// The address of a socket
typedef struct sockaddr_in mh_socket_address_t;

// The socket type
#ifndef WIN32
typedef int mh_socket_t;
#else
typedef SOCKET mh_socket_t;
#endif

// A function pointer type for mh_tcp_start
typedef void (*mh_on_connect_t)(mh_context_t *, mh_socket_t, mh_socket_address_t);

// Convert a socket address to a string
unsigned short mh_tcp_address_to_string(char *dest, mh_socket_address_t address, size_t size);

// Convert a string to a socket address
mh_socket_address_t mh_tcp_string_to_address(const char *str, unsigned short port);

// Start a TCP listener
_Noreturn void
mh_tcp_start(mh_context_t *context, mh_socket_address_t address, int max_clients, mh_on_connect_t on_connect);

#endif //MHSERV_MH_TCP_H
