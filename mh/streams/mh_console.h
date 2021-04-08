#ifndef MHSERV_MH_CONSOLE_H
#define MHSERV_MH_CONSOLE_H

#include "mh_stream.h"

// A structure that helps with writing to the console
typedef struct mh_console_static_write {
    void (*write)(mh_context_t* context, const char* str);
    void (*write_memory)(mh_context_t* context, mh_memory_t* memory);
} mh_console_static_write_t;

// A structure that helps with reading from the console
typedef struct mh_console_static_read {
    char* (*read)(mh_context_t* context, size_t size);
    void (*read_memory)(mh_context_t* context, mh_memory_t* memory);
} mh_console_static_read_t;

// The console
typedef struct mh_console {
    // Open the console streams
    struct mh_console (*open)();
    // Are the streams opened?
    bool opened;

    // Output stream
    mh_stream_t* out;
    // Input stream
    mh_stream_t* in;
    // Error stream
    mh_stream_t* err;

    // Output writer
    mh_console_static_write_t output;

    // Input reader
    mh_console_static_read_t input;

    // Error writer
    mh_console_static_write_t error;
} mh_console_t;

// Use mh_console.output.write_memory() instead
void mh_console_output_write_memory(mh_context_t* context, mh_memory_t* memory);

// Use mh_console.error.write_memory() instead
void mh_console_error_write_memory(mh_context_t* context, mh_memory_t* memory);

// Use mh_console.error.read_memory() instead
void mh_console_input_read_memory(mh_context_t* context, mh_memory_t* memory);

// Use mh_console.output.write() instead
void mh_console_output_write(mh_context_t* context, const char *str);

// Use mh_console.error.write() instead
void mh_console_error_write(mh_context_t* context, const char *str);

// Use mh_console.input.read() instead
char* mh_console_input_read(mh_context_t* context, size_t size);

// Use mh_console.open() instead
mh_console_t mh_console_open(mh_context_t* context);

// The console
static mh_console_t mh_console = {
        .open = mh_console_open,
        .opened = false,
        .output.write = mh_console_output_write,
        .output.write_memory = mh_console_output_write_memory,
        .input.read = mh_console_input_read,
        .input.read_memory = mh_console_input_read_memory,
        .error.write = mh_console_error_write,
        .error.write_memory = mh_console_error_write_memory
};

#endif //MHSERV_MH_CONSOLE_H