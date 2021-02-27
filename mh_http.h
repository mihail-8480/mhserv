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
typedef struct {
    mh_memory const *input;
    mh_request top;
    mh_headers* headers;
    struct sockaddr_in address;
} mh_http_request;

#define ECHO(x) mh_memory_write_string(body, x)
#define HEADER(x) mh_memory_write_string(header, x)

void mh_http_api(mh_memory* header, mh_memory* body, mh_http_request request);

void mh_http(int sock, mh_buffer* input, mh_headers* request_headers, struct sockaddr_in addr) {
    size_t total;
    for(total = 0; (total = mh_read_to_end(sock, input, total)) == input->size; mh_buffer_double(input));
    mh_request request;
    if (!mh_request_read(input, &request)) return;
    *request_headers = mh_headers_get(input);
    mh_memory header = mh_memory_new(1024);
    mh_memory body = mh_memory_new(1024);
    mh_memory request_body = (mh_memory){total, input};
    mh_memory_write_string(&body, "\n");
    mh_http_api(&header, &body, (mh_http_request){&request_body, request, request_headers, addr});
    send(sock, header.buffer.ptr, header.position, 0);
    send(sock, body.buffer.ptr, body.position, 0);
    free(header.buffer.ptr);
    free(body.buffer.ptr);

}

void mh_http_connect(int sock, struct sockaddr_in addr) {
    mh_buffer input = mh_buffer_new(1024);
    mh_headers headers;
    mh_http(sock, &input, &headers, addr);
    if (headers.list != NULL) {
        free(headers.list);
    }
    free(input.ptr);
    close(sock);
}
#endif //MHSERV_MH_HTTP_H
