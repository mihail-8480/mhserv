#include "mh_http.h"
#include <unistd.h>

// Stuff that isn't supposed to be shown to other people
typedef struct mh_http_request_private {
    mh_http_request_t base;
    mh_memory_t *request_memory;
    mh_stream_t *request_stream;
    size_t request_header_end;
    size_t iterations;
} mh_http_request_private_t;

typedef struct mh_http_listener {
    mh_tcp_listener_t base;
    size_t mh_http_copy_buffer_size;
    size_t mh_http_max_request_size;
    bool (*mh_http_error_handler)(mh_context_t *, const char *, void *);
    http_request_handler_t http_request_handler;
} mh_http_listener_t;


void mh_http_set_request_handler(mh_tcp_listener_t* listener, http_request_handler_t request_handler) {
    MH_THIS(mh_http_listener_t*, listener);
    this->http_request_handler = request_handler;
}

void mh_http_set_error_handler(mh_tcp_listener_t* listener, mh_error_handler_t handler) {
    MH_THIS(mh_http_listener_t*, listener);
    this->mh_http_error_handler = handler;
}

mh_http_request_t *mh_http_request_new(mh_context_t *context, mh_socket_address_t address, mh_memory_t *header) {
    MH_THIS(mh_http_request_private_t*, mh_context_allocate(context, sizeof(mh_http_request_private_t), true).ptr);

    // Read the request method
    mh_memory_t method = mh_memory_read_until(header, ' ');

    // Read the URL
    mh_memory_t url = mh_memory_read_until(header, ' ');

    // Read the HTTP version
    mh_memory_t version = mh_memory_read_until(header, '\r');

    // Skip the \n
    header->offset++;

    mh_memory_t *memory = mh_memory_new(context, sizeof(mh_memory_t) * 16, true);
    // Read the headers
    size_t count = 0;
    for (mh_memory_t single; (single = mh_memory_read_until(header, '\r')).size != 0; header->offset++, count++) {
        if (memory->size / sizeof(mh_memory_t) < count + 1) {
            mh_memory_resize(context, memory, memory->size * 2);
        }
        ((mh_memory_t *) memory->address)[count] = single;
    }

    // Put the headers into a map
    mh_map_t *headers = mh_map_new(context);
    for (size_t i = 0; i < count; i++) {
        mh_memory_t head = ((mh_memory_t *) memory->address)[i];
        mh_memory_t key = mh_memory_read_until(&head, ':');
        mh_memory_t value = mh_memory_reference(head.address + head.offset + 1, head.size - (head.offset + 1));
        mh_map_add(headers, key, value);
    }

    // Create the request
    *this = (mh_http_request_private_t) {
            .base.address = address,
            .base.method = method,
            .base.url = url,
            .base.version = version,
            .base.headers = headers
    };

    return (mh_http_request_t *) this;
}


// Figure out where the end of the header is
size_t http_find_end_of_headers(const mh_tcp_listener_t* listener, mh_memory_t *mem) {
    MH_THIS(mh_http_listener_t*, listener);
    // Turn the memory into a character array
    char *str = (char *) mem->address;
    // If it's less than 8 characters, it probably isn't there
    if (mem->size < 8) return 0;

    // Go from the current location (minus some characters back, there are bugs here probably) to the end - 3
    for (size_t i = mem->offset - this->mh_http_copy_buffer_size - (mem->offset - 5 > 0 ? 4 : 0); i < mem->offset - 3; i++) {
        // Read 4 characters per iteration to check for \r\n\r\n
        if (str[i] == '\r' && str[i + 1] == '\n' && str[i + 2] == '\r' && str[i + 3] == '\n') {
            // Return the index of the first character + 4
            return i + 4;
        }
    }
    return 0;
}

void mh_http_request_read_content(const mh_tcp_listener_t* listener, mh_stream_t *socket_stream, mh_http_request_t *request) {
    MH_THIS(mh_http_listener_t*, listener);
    mh_http_request_private_t *private = (mh_http_request_private_t *) request;
    while (private->request_memory->offset == private->iterations * this->mh_http_copy_buffer_size) {
        private->iterations++;
        mh_stream_copy_to(private->request_stream, socket_stream, this->mh_http_copy_buffer_size);
    }
    request->content = mh_memory_reference(private->request_memory->address + private->request_header_end,
                                           private->request_memory->offset - private->request_header_end);
}


void mh_http_on_connect(mh_tcp_listener_t* listener, mh_context_t *context, mh_socket_t socket, mh_socket_address_t address) {
    // If there is no request handler, ERROR!
    MH_THIS(mh_http_listener_t*, listener);
    if (this->http_request_handler == NULL) {
        mh_context_error(context, "A request handler is not set.", mh_http_on_connect);
        return;
    }

    // Set the error handler to the current context if there is one
    if (this->mh_http_error_handler != NULL) {
        mh_context_set_error_handler(context, this->mh_http_error_handler);
    }

    // Create the streams
    mh_stream_t *socket_stream = mh_socket_stream_new(context, socket);
    mh_stream_t *request_stream = mh_memory_stream_new(context, this->mh_http_copy_buffer_size, false);

    // Get the request_stream stream's memory
    mh_memory_t *request_memory = mh_memory_stream_get_memory(request_stream);

    // This is the offset where the header ends
    size_t request_header_end;

    // Read from the client until you encounter the header's end or until you hit the limit
    size_t iterations = 0;
    do {
        iterations++;
        mh_stream_copy_to(request_stream, socket_stream, this->mh_http_copy_buffer_size);
        request_header_end = http_find_end_of_headers(&this->base, request_memory);
        if (request_header_end != 0) {
            break;
        }
        if (request_memory->size >= this->mh_http_max_request_size) {
            break;
        }
    } while (request_memory->offset == iterations * this->mh_http_copy_buffer_size);

    // Split the request_stream memory into header and post
    mh_memory_t header = mh_memory_reference(request_memory->address,
                                             request_memory->offset - (request_memory->offset - request_header_end));
    mh_memory_t post = mh_memory_reference(request_memory->address + request_header_end,
                                           request_memory->offset - request_header_end);

    // Parse the request header
    mh_http_request_t *request = mh_http_request_new(context, address, &header);
    request->content = post;

    // Init some private fields
    mh_http_request_private_t *private = (mh_http_request_private_t *) request;
    private->request_memory = request_memory;
    private->request_stream = request_stream;
    private->request_header_end = request_header_end;

    // Call the request handler
    this->http_request_handler(&this->base, context, socket_stream, request);
}


mh_tcp_listener_t *mh_http_listener_new(mh_tcp_listener_t base) {
    MH_THIS(mh_http_listener_t*, mh_context_allocate(base.context, sizeof(mh_http_listener_t), true).ptr);
    this->base = base;
    this->base.on_connect = mh_http_on_connect;
    this->mh_http_copy_buffer_size = 128;
    this->mh_http_max_request_size = 16384;
    this->mh_http_error_handler = NULL;
    this->http_request_handler = NULL;
    return &this->base;
}