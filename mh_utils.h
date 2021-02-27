#include "include.h"
#ifndef MHSERV_MH_TYPES_H
#define MHSERV_MH_TYPES_H

// stole this from stack overflow
void mh_trim(char* s) {
    char *p = s;
    size_t l = strlen(p);
    while (isspace(p[l - 1])) {
        p[--l] = 0;
    }
    while (*p && isspace(*p)) {
        ++p, --l;
    }
    memmove(s, p, l + 1);
}

// Key/Value pair (of two strings)
typedef struct {
    char *key;
    char *value;
} mh_kv;

// Read all bytes (that fit) from a socket to a mh_buffer with an offset
size_t mh_read_to_end(int sock, mh_buffer* input, size_t offset) {
    return offset + read(sock, input->ptr + offset, input->size - offset);
}

#endif //MHSERV_MH_TYPES_H
