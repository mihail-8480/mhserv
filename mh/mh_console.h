#ifndef MHSERV_MH_CONSOLE_H
#define MHSERV_MH_CONSOLE_H

#include "mh_stream.h"
typedef struct mh_console_static_write {
    void (*write)(const char* str);
    void (*write_memory)(mh_memory_t* memory);
} mh_console_static_write_t;


typedef struct mh_console_static_read {
    char* (*read)(size_t size);
    void (*read_memory)(mh_memory_t* memory);
} mh_console_static_read_t;

typedef struct mh_console {
    struct mh_console (*open)();
    bool opened;
    mh_stream_t* _out;
    mh_stream_t* _in;
    mh_stream_t* _err;
    mh_console_static_write_t output;
    mh_console_static_read_t input;
    mh_console_static_write_t error;
    int argc;
    char **argv;
} mh_console_t;

void mh_console_output_write_memory(mh_memory_t* memory);

void mh_console_error_write_memory(mh_memory_t* memory);

void mh_console_input_read_memory(mh_memory_t* memory);

void mh_console_output_write(const char *str);

void mh_console_error_write(const char *str);

char* mh_console_input_read(size_t size);

mh_console_t mh_console_open(void);
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