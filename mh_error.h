#include "include.h"

#ifndef MHSERV_MH_ERROR_H
#define MHSERV_MH_ERROR_H
static inline void mh_error(bool cond) {
    if (!cond) {
        perror("Fatal error");
#ifndef MH_SURVIVE
    abort();
#endif
    }
}

static inline void mh_error_user(bool cond, const char* err, bool fatal) {
    if (!cond) {
        fprintf(stderr, "%s: %s\n", fatal ? "Fatal error" : "Error", err);
#ifndef MH_SURVIVE
        if (fatal) {
            abort();
        }
#endif
    }
}
#endif //MHSERV_MH_ERROR_H
