#include "include.h"
#include "mh_utils.h"

#ifndef MHSERV_MH_HEADERS_H
#define MHSERV_MH_HEADERS_H
size_t mh_headers_read(const mh_buffer* buf, mh_buffer* list_b) {
    char *str = buf->ptr;
    mh_kv *list = list_b->ptr;
    size_t list_i = 0;
    while(true) {
        str = strchr(str, '\n');
        str++;
        if (!strcmp(str, "\r\n") || !strcmp(str, "\n")) {
            return list_i;
        }
        char *next = strchr(str, ':');
        size_t key_size = next - str;
        char *key = malloc(key_size + 1);
        memcpy(key, str, key_size);
        key[key_size] = '\0';
        size_t value_size = strchr(next, '\n') - str - key_size;
        char *value = malloc(value_size + 1);
        memcpy(value, next+1, value_size);
        value[value_size] = '\0';
        mh_trim(value);
        if ((list_i + 2) * sizeof(mh_kv) > list_b->size) {
            mh_buffer_double(list_b);
        }
        list[list_i++] = (mh_kv){key, value};
    }
}

typedef struct {
    mh_kv* list;
    size_t count;
} mh_headers;

mh_headers mh_headers_get(const mh_buffer* buf) {
    mh_buffer headers = mh_buffer_new(32 * sizeof(mh_kv));
    size_t count = mh_headers_read(buf, &headers);
    return (mh_headers){headers.ptr, count};
}

#endif //MHSERV_MH_HEADERS_H
