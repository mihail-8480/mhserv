#include "mh_console.h"
#include "mh_stream_private.h"

void mh_console_stream_free(void* ptr) {
    free(ptr);
}

void mh_console_free(void* ptr) {
    mh_console_t* this = (mh_console_t*)ptr;
    // Free bunch of pointers
    mh_destructor_free(this->err);
    mh_destructor_free(this->in);
    mh_destructor_free(this->out);
    free(ptr);
}

mh_console_t mh_console_new(void) {
    mh_console_t console;

    // Get the standard streams
    console.out = mh_socket_stream_new(stdout->_fileno);
    console.in = mh_socket_stream_new(stdin->_fileno);
    console.err = mh_socket_stream_new(stderr->_fileno);
    console.destructor.free = mh_console_free;

    // Disable reading from stdin and stderr
    ((mh_stream_private_t *) console.out)->can_read = false;
    ((mh_stream_private_t *) console.err)->can_read = false;
    // Disable writing to stdin
    ((mh_stream_private_t *) console.in)->can_write = false;

    // Override the destructors
    ((mh_stream_private_t *) console.in)->base.destructor.free = mh_console_stream_free;
    ((mh_stream_private_t *) console.err)->base.destructor.free = mh_console_stream_free;
    ((mh_stream_private_t *) console.out)->base.destructor.free = mh_console_stream_free;
    return console;
}