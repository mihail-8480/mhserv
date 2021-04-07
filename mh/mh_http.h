#ifndef MHSERV_MH_HTTP_H
#define MHSERV_MH_HTTP_H

#include "mh_destructor.h"
#include "mh_memory.h"
#include "mh_tcp.h"
#include "mh_stream.h"

typedef struct mh_http_request {
    mh_destructor_t destructor;
    mh_memory_t method;
    mh_memory_t url;
    mh_memory_t version;
    mh_memory_t *headers;
    size_t headers_count;
    mh_socket_address_t address;
    mh_memory_t content;
} mh_http_request_t;
mh_http_request_t* mh_http_request_new(mh_socket_address_t address, mh_memory_t* header);

#define ENDL "\r\n"

// Why allocate stuff when you don't have to
#define ECHO(str) mh_stream_write_reference(socket_stream, str, sizeof(str)-1)

// The HTTP protocol
void mh_http(int socket, mh_socket_address_t address);

typedef void (*http_request_handler_t)(mh_stream_t *socket_stream, mh_http_request_t *request);

void mh_http_set_request_handler(http_request_handler_t request_handler);
#endif //MHSERV_MH_HTTP_H
