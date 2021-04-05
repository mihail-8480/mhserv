#include "include.h"
#include "mh_socket.h"
#include "mh_buffer.h"
#include "mh_memory.h"
#include "mh_utils.h"

#ifndef MHSERV_MH_REQUEST_H
#define MHSERV_MH_REQUEST_H
#define READ_AHEAD 1

// The request structure
typedef struct {
    // HTTP method
    char *method;
    // URL
    char *url;
    // HTTP version
    char *version;
    // The headers (might need trimming)
    char **headers;
    // The number of headers (including the last empty header)
    size_t header_count;
    // The address of the client
    struct sockaddr_in address;
} mh_request;

// Start reading from a socket and create a HTTP request struct
mh_request mh_request_new(int sock) {
    // Temp memory stream
    mh_memory memory = mh_memory_new(1024);
    // The memory where the headers will be located at
    mh_buffer headers = mh_buffer_new(sizeof(char*) * 32);
    // Where to continue reading
    size_t offset = 0;

    // Read the first line and make the request out of it
    mh_request request_top = {
            mh_trim(mh_read_until(sock, &memory, &offset, ' ', READ_AHEAD)),
            mh_trim(mh_read_until(sock, &memory, &offset, ' ', READ_AHEAD)),
            mh_trim(mh_read_until(sock, &memory, &offset, CRLF[0], READ_AHEAD))
    };
    // Read the headers
    char *header;
    size_t i = 0;
    do {
        header = mh_trim(mh_read_until(sock, &memory, &offset, CRLF[0], READ_AHEAD));
        mh_buffer_auto_double((i+1)*sizeof(char*),&headers);
        ((char**)headers.ptr)[i++] = header;
    } while (*header != '\0');

    // Free the temp memory stream
    free(memory.buffer.ptr);

    // Add the headers to the request
    request_top.headers = headers.ptr;
    request_top.header_count = i;
    return request_top;
}
#endif //MHSERV_MH_REQUEST_H
