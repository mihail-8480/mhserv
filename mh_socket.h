#include "include.h"
#include "mh_memory.h"

#ifndef MHSERV_MH_SOCKET_H
#define MHSERV_MH_SOCKET_H

// Fill a memory steam by reading bunch of bytes form a socket
void mh_read_ahead(int sock, mh_memory* memory, size_t count) {
    // Make sure there is enough space
    mh_buffer_auto_double(memory->position + count, &memory->buffer);
    // Read that amount of bytes
    ssize_t response = read(sock, memory->buffer.ptr + memory->position, count);
    // Report errors
    mh_error_user(response != -1, "Failed reading", false);
    // Increment the position
    memory->position += response < 0 ? 0 : response;
}

// Read until a character is reached
char *mh_read_until(int sock, mh_memory* memory, size_t* memory_offset, char chr, size_t increment) {
    size_t offset = 0;
    char *ptr = NULL;

    // Read ahead until the character is reached
    while(ptr == NULL) {
        mh_read_ahead(sock, memory, increment);
        offset = mh_memory_read_until(memory, chr, *memory_offset, &ptr);
    }

    // Change the memory offset
    *memory_offset = offset;

    return ptr;
}

#endif //MHSERV_MH_SOCKET_H
