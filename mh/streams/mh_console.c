#include "mh_console.h"
#include "mh_stream_private.h"

mh_console_t mh_console_open(mh_context_t* context) {
    if (mh_console.opened) return mh_console;

    // Get the standard streams
    mh_console.out = mh_file_stream_new(context, stdout, false);
    mh_console.in = mh_file_stream_new(context, stdin, false);
    mh_console.err = mh_file_stream_new(context, stderr, false);

    // Disable reading from stdin and stderr
    ((mh_stream_private_t *) mh_console.out)->can_read = false;
    ((mh_stream_private_t *) mh_console.err)->can_read = false;
    // Disable writing to stdin
    ((mh_stream_private_t *) mh_console.in)->can_write = false;

    // Disable seeking
    ((mh_stream_private_t *) mh_console.out)->can_seek = false;
    ((mh_stream_private_t *) mh_console.err)->can_seek = false;
    ((mh_stream_private_t *) mh_console.in)->can_seek = false;

    // Override the destructors
    ((mh_stream_private_t *) mh_console.in)->base.destructor.free = NULL;
    ((mh_stream_private_t *) mh_console.err)->base.destructor.free = NULL;
    ((mh_stream_private_t *) mh_console.out)->base.destructor.free = NULL;

    mh_console.opened = true;
    return mh_console;
}

void mh_console_output_write_memory(mh_context_t* context, mh_memory_t* memory) {
    if (!mh_console.opened) {
        mh_console = mh_console_open(context);
    }
    mh_stream_write(mh_console.out, memory, memory->size);
}


void mh_console_error_write_memory(mh_context_t* context, mh_memory_t* memory) {
    if (!mh_console.opened) {
        mh_console = mh_console_open(context);
    }
    mh_stream_write(mh_console.err, memory, memory->size);
}

void mh_console_input_read_memory(mh_context_t* context, mh_memory_t* memory) {
    if (!mh_console.opened) {
        mh_console = mh_console_open(context);
    }
    mh_stream_read(mh_console.in, memory, memory->size);
}

void mh_console_output_write(mh_context_t* context, const char *str) {
    if (!mh_console.opened) {
        mh_console = mh_console_open(context);
    }
    mh_stream_write_reference(mh_console.out, str, strlen(str));
}

void mh_console_error_write(mh_context_t* context, const char *str) {
    if (!mh_console.opened) {
        mh_console = mh_console_open(context);
    }
    mh_stream_write_reference(mh_console.err, str, strlen(str));
}

char* mh_console_input_read(mh_context_t* context, size_t size) {
    if (!mh_console.opened) {
        mh_console = mh_console_open(context);
    }
    char* buffer = mh_context_allocate(context, size, false).ptr;
    mh_memory_t reference = mh_memory_reference(buffer,  size);
    mh_stream_read(mh_console.in, &reference, size);
    return buffer;
}
