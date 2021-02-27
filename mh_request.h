#include "include.h"
#include "mh_buffer.h"

#ifndef MHSERV_MH_REQUEST_H
#define MHSERV_MH_REQUEST_H

// The request header's header.
typedef struct {
    // Request type
    char type[10];

    // The URL
    char uri[2000];

    // HTTP version
    char version[5];
} mh_request;

// Read the request information from the buffer
int mh_request_read(const mh_buffer* buf, mh_request* request) {
    // Not a very smart way to do it, but it works
    int read = sscanf((char*)buf->ptr, "%s %s HTTP/%s\n", request->type, request->uri, request->version);
    // Report the error, but do not crash
    mh_error_user(read, "Not a valid HTTP request", false);
    return read;
}
#endif //MHSERV_MH_REQUEST_H
