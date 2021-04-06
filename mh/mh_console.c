#include "mh_console.h"
#include "mh_stream_private.h"

void mh_console_stream_free(void* ptr) {
    free(ptr);
}

void mh_console_free(void* ptr) {
    mh_console_t* this = (mh_console_t*)ptr;
    // Free bunch of pointers
    mh_destructor_free(this->_err);
    mh_destructor_free(this->_in);
    mh_destructor_free(this->_out);
    free(ptr);
}

void mh_console_output_write(const char *str) {
    mh_stream_write_reference(mh_internal_console._out, str, strlen(str));
}

void mh_console_error_write(const char *str) {
    mh_stream_write_reference(mh_internal_console._err, str, strlen(str));
}

char* mh_console_input_read(size_t size) {
    char* stream = malloc(sizeof(size));
    mh_memory_t reference = mh_memory_reference(stream,  size);
    mh_stream_read(mh_internal_console._in, &reference, size);
    return stream;
}

mh_console_t mh_console_open(void) {
    if (mh_internal_console.opened) return mh_internal_console;
    // Get the standard streams
    mh_internal_console._out = mh_socket_stream_new(stdout->_fileno);
    mh_internal_console._in = mh_socket_stream_new(stdin->_fileno);
    mh_internal_console._err = mh_socket_stream_new(stderr->_fileno);
    mh_internal_console.destructor.free = mh_console_free;

    // Disable reading from stdin and stderr
    ((mh_stream_private_t *) mh_internal_console._out)->can_read = false;
    ((mh_stream_private_t *) mh_internal_console._err)->can_read = false;
    // Disable writing to stdin
    ((mh_stream_private_t *) mh_internal_console._in)->can_write = false;

    // Override the destructors
    ((mh_stream_private_t *) mh_internal_console._in)->base.destructor.free = mh_console_stream_free;
    ((mh_stream_private_t *) mh_internal_console._err)->base.destructor.free = mh_console_stream_free;
    ((mh_stream_private_t *) mh_internal_console._out)->base.destructor.free = mh_console_stream_free;

    mh_internal_console.output.write = mh_console_output_write;
    mh_internal_console.error.write = mh_console_output_write;
    mh_internal_console.input.read = mh_console_input_read;

    mh_internal_console.opened = true;
    return mh_internal_console;
}