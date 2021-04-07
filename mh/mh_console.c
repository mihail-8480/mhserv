#include "mh_console.h"
#include "mh_stream_private.h"

mh_console_t mh_console_open(void) {
    if (mh_console.opened) return mh_console;

    // Get the standard streams
    mh_console._out = mh_file_stream_new(stdout, false);
    mh_console._in = mh_file_stream_new(stdin, false);
    mh_console._err = mh_file_stream_new(stderr, false);

    // Disable reading from stdin and stderr
    ((mh_stream_private_t *) mh_console._out)->can_read = false;
    ((mh_stream_private_t *) mh_console._err)->can_read = false;
    // Disable writing to stdin
    ((mh_stream_private_t *) mh_console._in)->can_write = false;

    // Disable seeking
    ((mh_stream_private_t *) mh_console._out)->can_seek = false;
    ((mh_stream_private_t *) mh_console._err)->can_seek = false;
    ((mh_stream_private_t *) mh_console._in)->can_seek = false;

    // Override the destructors
    ((mh_stream_private_t *) mh_console._in)->base.destructor.free = NULL;
    ((mh_stream_private_t *) mh_console._err)->base.destructor.free = NULL;
    ((mh_stream_private_t *) mh_console._out)->base.destructor.free = NULL;

    mh_console.opened = true;
    return mh_console;
}

void mh_console_output_write(const char *str) {
    if (!mh_console.opened) {
        mh_console = mh_console_open();
    }
    mh_stream_write_reference(mh_console._out, str, strlen(str));
}

void mh_console_error_write(const char *str) {
    if (!mh_console.opened) {
        mh_console = mh_console_open();
    }
    mh_stream_write_reference(mh_console._err, str, strlen(str));
}

char *mh_console_input_read(size_t size) {
    if (!mh_console.opened) {
        mh_console = mh_console_open();
    }
    char* stream = malloc(sizeof(size));
    mh_memory_t reference = mh_memory_reference(stream,  size);
    mh_stream_read(mh_console._in, &reference, size);
    return stream;
}
