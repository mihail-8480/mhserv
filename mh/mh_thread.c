#include "mh_thread.h"
#include <pthread.h>

void mh_thread_create(mh_thread_action_t action, void *args) {
    // Create the new thread and set it's arguments
    pthread_attr_t attr;
    pthread_t thread;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 1);
    pthread_create(&thread, &attr, action, args);
    pthread_attr_destroy(&attr);
}

void mh_thread_exit(void *ret) {
    pthread_exit(ret);
}
