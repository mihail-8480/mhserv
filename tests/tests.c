#include "tests.h"
#include <stdio.h>
#include <stdlib.h>

void mh_test_run(mh_test_t test, const char* name) {
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

int main(void) {
    MH_TEST_RUN(context_allocation_test);
    MH_TEST_RUN(map_test);
    MH_TEST_RUN(stream_test);
    MH_TEST_RUN(create_thread_test);
}