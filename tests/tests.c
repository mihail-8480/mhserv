#include "../mh/mh_context.h"
#include "../mh/collections/mh_map.h"
#include "../mh/streams/mh_stream.h"
#include "../mh/mh_thread.h"
#include <stdio.h>
typedef bool (*mh_test_t)(mh_context_t* context);

bool map_test(mh_context_t* context) {
    mh_map_t* map = mh_map_new(context);
    mh_memory_t keys[] = {
            mh_memory_reference("Hello", 5),
            mh_memory_reference("Hello1", 6),
            mh_memory_reference("Hello2", 6),
            mh_memory_reference("Hello3", 6),
    };
    mh_memory_t values[] = {
            mh_memory_reference("1", 1),
            mh_memory_reference("2", 1),
            mh_memory_reference("3", 1),
            mh_memory_reference("4", 1)
    };

    for(size_t i = 0; i < 4; i++) {
        mh_map_add(map, keys[i], values[i]);
    }

    for(size_t i = 0; i < 4; i++) {
        if (!mh_map_contains(map, keys[i])) {
            return false;
        }
    }

    for(size_t i = 0; i < 4; i++) {
        mh_memory_t mem = mh_map_get(map, keys[i]);
        if (mem.address == NULL) {
            return false;
        }
        if (!mh_memory_is_equal(mem, values[i])) {
            return false;
        }
    }

    return true;
}

bool context_allocation_test(mh_context_t* context) {
    for(size_t i = 0; i < 1000; i++) {
        mh_context_allocation_reference_t allocation = mh_context_allocate(context, 1000, false);
        void* ptr = mh_context_reallocate(context, allocation, 2000);
        if (allocation.ptr == NULL) return false;
        if (ptr == NULL) return false;
    }
    return true;
}

bool stream_test(mh_context_t* context) {
    mh_stream_t* stream = mh_memory_stream_new(context, 0, false);
    mh_memory_t memory = mh_memory_reference("Hello", 5);
    mh_stream_write(stream, &memory, memory.size);
    mh_stream_seek(stream, 0);
    char mem[5];
    mh_memory_t mem_ref = mh_memory_reference(mem, 5);
    mh_stream_read(stream, &mem_ref, 5);
    return mh_memory_is_equal(mem_ref, memory);
}

void* do_nothing() {

}

bool create_thread_test(mh_context_t* context) {
    return false;
    for(int i = 0; i < 100; i++) {
        mh_thread_create(do_nothing, NULL);
    }
    return true;
}

static inline void mh_test_run(mh_test_t test, const char* name) {
    printf("Running %s", name);
    mh_context_t *context = mh_start();
    bool res = test(context);
    mh_end(context);
    if (res) {
        printf(" [PASSED]\n");
    } else {
        printf(" [FAILED]\n");
        exit(1);
    }
}

#define MH_TEST_RUN(test) mh_test_run(test, #test)

int main(void) {
    MH_TEST_RUN(context_allocation_test);
    MH_TEST_RUN(map_test);
    MH_TEST_RUN(stream_test);
    MH_TEST_RUN(create_thread_test);
}