#include "include.h"
#ifndef MHSERV_MH_TYPES_H
#define MHSERV_MH_TYPES_H

// stole this from stack overflow
char* mh_trim(char* s) {
    char *p = s;
    size_t l = strlen(p);
    while (isspace(p[l - 1])) {
        p[--l] = 0;
    }
    while (*p && isspace(*p)) {
        ++p, --l;
    }
    memmove(s, p, l + 1);
    return s;
}

#endif //MHSERV_MH_TYPES_H
