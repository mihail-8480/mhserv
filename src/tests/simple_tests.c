#include <collections/mh_map.h>
#include <streams/mh_stream.h>
#include <mh_thread.h>
#include "tests.h"


void* do_nothing() {

}

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
bool create_thread_test(mh_context_t* context) {
    for(int i = 0; i < 100; i++) {
        mh_thread_create(do_nothing, NULL);
    }
    return true;
}

bool context_create_test(mh_context_t *context) {
    for (int i = 0; i < 10000; i++) {
        mh_context_t *c = mh_start();
        context_allocation_test(c);
        mh_end(c);
    }
    return true;
}
