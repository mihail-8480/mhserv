#include "include.h"
#include "mh_request.h"
#include "mh_utils.h"
#include "mh_buffer.h"
#include "mh_error.h"
#include "mh_request.h"
#include "mh_utils.h"
#include "mh_memory.h"

#ifndef MHSERV_MH_HTTP_H
#define MHSERV_MH_HTTP_H

// For simplicity
#define ECHO(x) mh_memory_write_string(body, x)
#define HEADER(x) mh_memory_write_string(header, x)
#define DO_NOT_SEND(x) free(x->buffer.ptr)

// This needs to be implemented in main if you want to use HTTP
void mh_http_api(mh_memory* header, mh_memory* body, mh_request* request);


// To connect the TCP listener with the HTTP handler
void mh_http_connect(int sock, struct sockaddr_in addr) {
    // Read the request header
    mh_request request = mh_request_new(sock);
    request.address = addr;

    // Create the response header and body
    mh_memory header = mh_memory_new(1024);
    mh_memory body = mh_memory_new(1024);
    mh_memory_write_string(&body, "\n");

    // Call the user defined function
    mh_http_api(&header, &body, &request);

    // Write the response header if that isn't handled by the user
    if (header.buffer.ptr != NULL) {
        // If there is a body, write the content length to prevent a "connection reset" problem
        if (body.buffer.ptr != NULL) {
            char content_header[50];
            sprintf(content_header, "Content-Length: %zu\n", body.position-1);
            mh_memory_write_string(&header, content_header);
        }
        send(sock, header.buffer.ptr, header.position, 0);
        free(header.buffer.ptr);
    }

    // Write the body (if there is one)
    if (body.buffer.ptr != NULL) {
        send(sock, body.buffer.ptr, body.position, 0);
        free(body.buffer.ptr);
    }

    // Free resources
    free(request.method);
    free(request.version);
    free(request.url);
    for(size_t i = 0; i < request.header_count; i++){
        free(request.headers[i]);
    }
    free(request.headers);
    close(sock);
}
#endif //MHSERV_MH_HTTP_H
