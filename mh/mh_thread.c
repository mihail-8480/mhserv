#include "mh_thread.h"
#ifndef WIN32
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
    // Exit from the thread
    pthread_exit(ret);
}
#else
#include <windows.h>
void mh_thread_create(mh_thread_action_t action, void *args) {
    // Create the new thread and set it's arguments
    DWORD id;
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)action, args, 0 , &id);
}

void mh_thread_exit(void *ret) {
    // Exit from the thread
    ExitThread((DWORD)ret);
}
#endif