#ifndef MHSERV_TESTS_H
#define MHSERV_TESTS_H
#include "../mh/mh_context.h"
typedef bool (*mh_test_t)(mh_context_t* context);
void mh_test_run(mh_test_t test, const char* name);
#define MH_TEST_RUN(test) mh_test_run(test, #test)
int main(void);

// in simple_tests.c

bool map_test(mh_context_t* context);
bool context_allocation_test(mh_context_t* context);
bool stream_test(mh_context_t* context);
bool create_thread_test(mh_context_t* context);

#endif //MHSERV_TESTS_H
