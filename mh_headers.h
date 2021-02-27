#include "include.h"
#include "mh_utils.h"

#ifndef MHSERV_MH_HEADERS_H
#define MHSERV_MH_HEADERS_H

// Read the headers
size_t mh_headers_read(const mh_buffer* buf, mh_buffer* list_b) {
    // TODO: rework this

    // Turn the input buffer into a character pointer
    char *str = buf->ptr;

    // Turn the second buffer into a key/value pair pointer
    mh_kv *list = list_b->ptr;

    // Keep track of where you are located (maybe use memory stream?)
    size_t list_i = 0;

    while(true) {
        // Find the next new line and move to it
        str = strchr(str, '\n');
        // Increment by 1 so we don't start with a new line
        str++;

        // If the line is still empty it means that we have reached the end of the headers
        // We should return here
        if (!strcmp(str, "\r\n") || !strcmp(str, "\n")) {
            return list_i;
        }

        // The key is from the \n to the next :
        char *next = strchr(str, ':');
        size_t key_size = next - str;
        char *key = malloc(key_size + 1);
        memcpy(key, str, key_size);
        key[key_size] = '\0';

        // The value is from the : to the next \n
        size_t value_size = strchr(next, '\n') - str - key_size;
        char *value = malloc(value_size + 1);
        memcpy(value, next+1, value_size);
        value[value_size] = '\0';

        // Gotta trim to remove the stupid \r\n shit and the space after the :
        mh_trim(value);

        // If there isn't enough space to fit, expand the buffer
        if ((list_i + 2) * sizeof(mh_kv) > list_b->size) {
            mh_buffer_double(list_b);
        }
        // Add the key/value pair to the buffer
        list[list_i++] = (mh_kv){key, value};
    }
}

// Probably should replace with a memory stream, this is fine too tbh
typedef struct {
    mh_kv* list;
    size_t count;
} mh_headers;

// A wrapper function that is more user friendly
mh_headers mh_headers_get(const mh_buffer* buf) {
    mh_buffer headers = mh_buffer_new(32 * sizeof(mh_kv));
    size_t count = mh_headers_read(buf, &headers);
    return (mh_headers){headers.ptr, count};
}

#endif //MHSERV_MH_HEADERS_H
