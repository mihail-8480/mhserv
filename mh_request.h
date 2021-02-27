#include "include.h"
#include "mh_buffer.h"

#ifndef MHSERV_MH_REQUEST_H
#define MHSERV_MH_REQUEST_H
typedef struct {
    char type[10];
    char uri[2000];
    char version[5];
} mh_request;

int mh_request_read(const mh_buffer* buf, mh_request* request) {
    int read = sscanf((char*)buf->ptr, "%s %s HTTP/%s\n", request->type, request->uri, request->version);
    mh_error_user(read, "Not a valid HTTP request", false);
    return read;
}
#endif //MHSERV_MH_REQUEST_H
