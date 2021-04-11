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

// End of line
#define MH_ENDL "\r\n"

// The entry point of a http handler library
#define MH_HTTP_HANDLE mh_http_handle

// Turn an identifier into a string
#define MH_STRINGIFY(x) #x

// Turn a value of a macro into a string
#define MH_MACRO_STRINGIFY(x) MH_STRINGIFY(x)

// The socket stream variable
#define MH_SOCKET_STREAM socket_stream

// The request variable
#define MH_REQUEST request

// The default arguments of a HTTP request handler
#define MH_HTTP_DEFAULT_ARGS mh_context_t* context, mh_stream_t* MH_SOCKET_STREAM, mh_http_request_t* MH_REQUEST

// Write a string literal into MH_SOCKET_STREAM
#define MH_ECHO(str) mh_stream_write_reference(MH_SOCKET_STREAM, str, sizeof(str)-1)
// Write a string into MH_SOCKET_STREAM
#define MH_ECHO_STR(str) mh_stream_write_reference(MH_SOCKET_STREAM, str, strlen(str))

// Get a header from MH_REQUEST
#define MH_HEADER(str) mh_map_get(MH_REQUEST->headers, MH_REF_CONST(str))

// The HTTP protocol
void mh_http(mh_context_t* context, int socket, mh_socket_address_t address);

// A HTTP request handler
typedef void (*http_request_handler_t)(MH_HTTP_DEFAULT_ARGS);

// Set a request handler
void mh_http_set_request_handler(http_request_handler_t request_handler);

// Set an error handler

void mh_http_set_error_handler(mh_error_handler_t handler);

// Finish reading the entire post content into memory
void mh_http_request_read_content(mh_stream_t* socket_stream, mh_http_request_t* request);
#endif //MHSERV_MH_HTTP_H
