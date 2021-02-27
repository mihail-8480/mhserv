#include "include.h"
#include "mh_request.h"
#include "mh_utils.h"
#include "mh_buffer.h"
#include "mh_error.h"
#include "mh_request.h"
#include "mh_utils.h"
#include "mh_headers.h"
#include "mh_memory.h"

#ifndef MHSERV_MH_HTTP_H
#define MHSERV_MH_HTTP_H

// Because mh_request isn't enough, here's a proper request struct
typedef struct {
    // The request bytes
    mh_memory const *input;
    // The top part of the header
    mh_request top;
    // The HTTP headers
    mh_headers* headers;
    // The client address
    struct sockaddr_in address;
} mh_http_request;

// For simplicity
#define ECHO(x) mh_memory_write_string(body, x)
#define HEADER(x) mh_memory_write_string(header, x)

// This needs to be implemented in main if you want to use HTTP
void mh_http_api(mh_memory* header, mh_memory* body, mh_http_request request);

// Does basic HTTP stuff
void mh_http(int sock, mh_buffer* input, mh_headers* request_headers, struct sockaddr_in addr) {
    // Read the whole request
    // TODO: Read the headers first, and add a function to read the rest
    size_t total;
    for(total = 0; (total = mh_read_to_end(sock, input, total)) == input->size; mh_buffer_double(input));

    // Read the request top, if it's not valid, get away from the client
    mh_request request;
    if (!mh_request_read(input, &request)) return;

    // Read the request headers
    *request_headers = mh_headers_get(input);

    // Create the response headers and response body memory streams
    mh_memory header = mh_memory_new(1024);
    mh_memory body = mh_memory_new(1024);

    // The request text should be readable too
    mh_memory request_text = (mh_memory){total, input};

    // Write an empty line to separate the body from the header
    mh_memory_write_string(&body, "\n");

    // Call the user defined function with the request
    mh_http_api(&header, &body, (mh_http_request){&request_text, request, request_headers, addr});

    // Send the response
    send(sock, header.buffer.ptr, header.position, 0);
    send(sock, body.buffer.ptr, body.position, 0);

    // Free the response memory
    free(header.buffer.ptr);
    free(body.buffer.ptr);

}

// To connect the TCP listener with the HTTP handler
void mh_http_connect(int sock, struct sockaddr_in addr) {
    // Create an input buffer to read into
    mh_buffer input = mh_buffer_new(1024);
    // Create the headers, so we can free them if the handler returns too early
    mh_headers headers;

    // Call the handler
    mh_http(sock, &input, &headers, addr);

    // Free everything
    if (headers.list != NULL) {
        free(headers.list);
    }
    free(input.ptr);
    close(sock);
}
#endif //MHSERV_MH_HTTP_H
