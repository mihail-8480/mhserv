#include "mh_thread.h"
#include <pthread.h>

void mh_thread_create(void *(*callback)(void *), void *args) {
    pthread_attr_t attr;
    pthread_t thread;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 1);
    pthread_create(&thread, &attr, callback, args);
    pthread_attr_destroy(&attr);
}
