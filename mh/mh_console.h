#ifndef MHSERV_MH_CONSOLE_H
#define MHSERV_MH_CONSOLE_H

#include "mh_stream.h"
typedef struct mh_console_static_write {
    void (*write)(const char* str);
} mh_console_static_write_t;


typedef struct mh_console_static_read {
    char* (*read)(size_t size);
} mh_console_static_read_t;

typedef struct mh_console {
    mh_destructor_t destructor;
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

mh_console_t mh_console_open(void);
static mh_console_t mh_internal_console = { .open = mh_console_open, .opened = false };

#ifdef MH_MAIN
int mh_main(mh_console_t console);

int main(int argc, char *argv[]) {
    mh_internal_console = mh_internal_console.open();
    mh_internal_console.argc = argc;
    mh_internal_console.argv = argv;
    return mh_main(mh_internal_console);
}
#endif

#endif //MHSERV_MH_CONSOLE_H