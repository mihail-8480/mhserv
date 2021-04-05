#include "mh/mh_stream.h"
#include "stdio.h"

void test_stream(void) {
    mh_stream_t* stream = mh_memory_stream_new(0, false);
    mh_stream_write(stream, "Hello", 5);
    printf("pos: %zu/%zu\n", mh_stream_get_position(stream), mh_stream_get_size(stream));
    mh_stream_write(stream, "Hello", 5);
    printf("pos: %zu/%zu\n", mh_stream_get_position(stream), mh_stream_get_size(stream));
    mh_stream_write(stream, "Hello", 5);
    printf("pos: %zu/%zu\n", mh_stream_get_position(stream), mh_stream_get_size(stream));
    mh_stream_write(stream, "Hello", 5);
    printf("pos: %zu/%zu\n", mh_stream_get_position(stream), mh_stream_get_size(stream));
    mh_stream_write(stream, "Hello", 5);
    printf("pos: %zu/%zu\n", mh_stream_get_position(stream), mh_stream_get_size(stream));
    mh_stream_write(stream, "", 1);
    size_t size = mh_stream_get_position(stream);
    mh_stream_seek(stream, 0);
    mh_stream_t* stream1 = mh_memory_stream_new(size, true);
    mh_stream_copy_to(stream1, stream, size);
    mh_stream_free(stream);
    mh_stream_seek(stream1, 0);
    char* string = mh_stream_read(stream1, size);
    puts(string);
    free(string);
    mh_stream_free(stream1);
}

int main(void) {
    test_stream();
}
