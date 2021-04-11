#ifndef MHSERV_MH_HTTP_H
#define MHSERV_MH_HTTP_H

#include "mh_tcp.h"
#include "../mh_memory.h"
#include "../streams/mh_stream.h"
#include "../collections/mh_map.h"

// A http request
typedef struct mh_http_request {
    mh_memory_t method;
    mh_memory_t url;
    mh_memory_t version;
    mh_map_t* headers;
    mh_socket_address_t address;
    mh_memory_t content;
} mh_http_request_t;

// The HTTP protocol
void mh_http(mh_context_t* context, int socket, mh_socket_address_t address);


// A HTTP request handler
typedef void (*http_request_handler_t)(mh_context_t*, mh_stream_t*, mh_http_request_t*);

// Set a request handler
void mh_http_set_request_handler(http_request_handler_t request_handler);

// Set an error handler

void mh_http_set_error_handler(mh_error_handler_t handler);

// Finish reading the entire post content into memory
void mh_http_request_read_content(mh_stream_t* socket_stream, mh_http_request_t* request);
#endif //MHSERV_MH_HTTP_H
