#include "mh_http.h"
#include "mh_stream.h"
#include "mh_error.h"
#include <unistd.h>
#include <string.h>

// The size of the buffer that is used while copying
const size_t mh_http_copy_buffer_size = 128;
// The maximal size of a request
const size_t mh_http_max_request_size = 16384;

void mh_http_request_free(void *ptr) {
    mh_http_request_t* self = (mh_http_request_t*)ptr;
    for(size_t i = 0; i < self->headers_count; i++) {
        free(self->headers[i]);
    }
    free(self->headers);
    free(self->version);
    free(self->url);
    free(self->method);
    free(self);
}

mh_http_request_t *mh_http_request_new(mh_socket_address address, mh_memory_t *header) {
    mh_http_request_t* request = calloc(1, sizeof(mh_http_request_t));

    // Read the request method
    char *method = mh_memory_read_until(header, ' ');

    // Read the URL
    char *url = mh_memory_read_until(header, ' ');

    // Read the HTTP version
    char *version = mh_memory_read_until(header, '\r');

    // Skip the \n
    header->offset++;

    mh_memory_t *memory = mh_memory_new(sizeof(char*)*16, true);
    // Read the headers
    size_t count = 0;
    for (char *single = NULL; (single = mh_memory_read_until(header, '\r')) != NULL; header->offset++, count++) {
        if (memory->size/sizeof(char*) < count + 1) {
            mh_memory_resize(memory, memory->size*2);
        }
        ((char**)memory->address)[count] = single;
    }
    *request = (mh_http_request_t) {
            .address = address,
            .method = method,
            .url = url,
            .version = version,
            .headers = (char**)memory->address,
            .headers_count = count,
            .destructor = mh_http_request_free
    };

    free(memory);
    return request;
}


// Figure out where the end of the header is
size_t http_find_end_of_headers(mh_memory_t *mem) {
    // Turn the memory into a character array
    char* str = (char*)mem->address;
    // If it's less than 8 characters, it probably isn't there
    if (mem->size < 8) return 0;

    // Go from the current location (minus some characters back, there are bugs here probably) to the end - 3
    for(size_t i = mem->offset - mh_http_copy_buffer_size - (mem->offset - 5 > 0 ? 4 : 0); i < mem->offset - 3; i++) {
        // Read 4 characters per iteration to check for \r\n\r\n
        if (str[i] == '\r' && str[i + 1] == '\n' && str[i + 2] == '\r' && str[i + 3] == '\n') {
            // Return the index of the first character + 4
            return i + 4;
        }
    }
    return 0;
}

http_request_handler_t http_request_handler = NULL;

void mh_http_set_request_handler(http_request_handler_t request_handler) {
    http_request_handler = request_handler;
}
void mh_http(int socket, mh_socket_address address) {
    if (http_request_handler == NULL) {
        close(socket);
        mh_error_report("A request handler is not set.");
    }

    // Create the streams
    mh_stream_t* socket_stream = mh_socket_stream_new(socket);
    mh_stream_t* request_stream = mh_memory_stream_new(mh_http_copy_buffer_size, false);

    // Create a destructor array
    mh_destructor_t* destructor = mh_destructor_array_new((mh_destructor_t*[]) {
            &socket_stream->destructor, &request_stream->destructor, NULL}, 3);

    // Get the request_stream stream's memory
    mh_memory_t* request_memory = mh_memory_stream_get_memory(request_stream);

    // This is the offset where the header ends
    size_t request_header_end;

    // Read from the client until you encounter the header's end or until you hit the max request_stream size limit
    size_t iterations = 0;
    do {
        iterations++;
        mh_stream_copy_to(request_stream, socket_stream, mh_http_copy_buffer_size);
        request_header_end = http_find_end_of_headers(request_memory);
        if (request_header_end != 0) {
            break;
        }
        if (request_memory->size >= mh_http_max_request_size) {
            break;
        }
    } while(request_memory->offset == iterations * mh_http_copy_buffer_size);

    // If you didn't encounter the header's end for some reason, complain
    if (request_header_end == 0) {
        mh_error_report_safe("Could not find end of header in request_stream.", destructor);
        return;
    }

    // Split the request_stream memory into header and post
    mh_memory_t header = mh_memory_reference(request_memory->address, request_memory->offset - (request_memory->offset - request_header_end));
    mh_memory_t post = mh_memory_reference(request_memory->address + request_header_end, request_memory->offset - request_header_end);

    // Parse the request header
    mh_http_request_t *request = mh_http_request_new(address, &header);
    mh_destructor_array_set(destructor, 2, &request->destructor);


    // If you are supposed to, read the entire post request_stream
    if (strcmp(request->method, "POST") == 0) {
        while (request_memory->offset == iterations * mh_http_copy_buffer_size) {
            iterations++;
            mh_stream_copy_to(request_stream, socket_stream, mh_http_copy_buffer_size);
        }
        post = mh_memory_reference(request_memory->address + request_header_end, request_memory->offset - request_header_end);
    }

    request->content = post;

    // Call the request handler
    http_request_handler(socket_stream, request);

    // Free the memory
    mh_destructor_free(destructor);
}