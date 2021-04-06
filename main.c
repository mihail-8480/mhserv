#include <string.h>
#include "mh/mh_tcp.h"
#include "mh/mh_stream.h"
#include "mh/mh_error.h"
#define ENDL "\r\n"

// The size of the buffer that is used while copying
const size_t copy_buffer_size = 128;
// The maximal size of a request
const size_t max_request_size = 16384;
// Should we read the post request?
bool finish_reading_post = false;


// Why allocate stuff when you don't have to
#define ECHO(str) mh_stream_write_reference(socket_stream, str, sizeof(str)-1)

// Figure out where the end of the header is
static inline size_t end_of_headers(mh_memory_t* mem) {
    // Turn the memory into a character array
    char* str = (char*)mem->address;
    // If it's less than 8 characters, it probably isn't there
    if (mem->size < 8) return 0;

    // Go from the current location (minus some characters back, there are bugs here probably) to the end - 3
    for(size_t i = mem->offset - copy_buffer_size - (mem->offset - 5 > 0 ? 4 : 0); i < mem->offset-3; i++) {
        // Read 4 characters per iteration to check for \r\n\r\n
        if (str[i] == '\r' && str[i + 1] == '\n' && str[i + 2] == '\r' && str[i + 3] == '\n') {
            // Return the index of the first character + 4
            return i + 4;
        }
    }
    return 0;
}

void on_connect(int socket, mh_socket_address address) {
    // Create the streams
    mh_stream_t* socket_stream = mh_socket_stream_new(socket);
    mh_stream_t* request_stream = mh_memory_stream_new(copy_buffer_size, false);

    // Get the request_stream stream's memory
    mh_memory_t* request_memory = mh_memory_stream_get_memory(request_stream);

    // This is the offset where the header ends
    size_t request_header_end;

    // Read from the client until you encounter the header's end or until you hit the max request_stream size limit
    size_t iterations = 0;
    do {
        iterations++;
        mh_stream_copy_to(request_stream, socket_stream, copy_buffer_size);
        request_header_end = end_of_headers(request_memory);
        if (request_header_end != 0) {
            break;
        }
        if (request_memory->size >= max_request_size) {
            break;
        }
    } while(request_memory->offset == iterations * copy_buffer_size);

    // If you didn't encounter the header's end for some reason, complain
    if (request_header_end == 0) {
        mh_error_report("Could not find end of header request_stream.");
    }

    // Split the request_stream memory into header and post
    mh_memory_t header = mh_memory_reference(request_memory->address, request_memory->offset - (request_memory->offset - request_header_end));
    mh_memory_t post = mh_memory_reference(request_memory->address + request_header_end, request_memory->offset - request_header_end);

    // If you are supposed to, read the entire post request_stream
    if (finish_reading_post) {
        while (request_memory->offset == iterations * copy_buffer_size) {
            iterations++;
            mh_stream_copy_to(request_stream, socket_stream, copy_buffer_size);
        }
        post = mh_memory_reference(request_memory->address + request_header_end, request_memory->offset - request_header_end);
    }

    // Write some stuff to the server
    ECHO("HTTP/1.1 200 OK" ENDL);
    ECHO("Content-Type: text/html; charset=UTF-8" ENDL);
    ECHO("Connection: close" ENDL);
    ECHO(ENDL);
    ECHO("<h1>KEK</h1>");

    // Free the memory
    mh_stream_free(socket_stream);
    mh_stream_free(request_stream);
}
int main(void) {
    // Start a TCP server on port 8080, with 32 max clients and with the on_connect method
    mh_tcp_start(8080, 32, on_connect);
}
