#ifndef MHSERV_MH_CONSOLE_H
#define MHSERV_MH_CONSOLE_H

#include "mh_stream.h"

typedef struct mh_console {
    mh_destructor_t destructor;
    mh_stream_t* out;
    mh_stream_t* in;
    mh_stream_t* err;

} mh_console_t;
// Create the standard streams
mh_console_t mh_console_new(void);

#endif //MHSERV_MH_CONSOLE_H
