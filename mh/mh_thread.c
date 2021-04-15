#include "mh_thread.h"
#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#endif
void mh_thread_create(mh_thread_action_t action, void *args) {
    // Create the new thread and set it's arguments
#ifndef WIN32
    pthread_attr_t attr;
    pthread_t thread;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 1);
    pthread_create(&thread, &attr, action, args);
    pthread_attr_destroy(&attr);
#else
    DWORD id;
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)action, args, 0 , &id);
#endif
}

void mh_thread_exit(void *ret) {
    // Exit from the thread
#ifndef WIN32
    pthread_exit(ret);
#else
    ExitThread(ret);
#endif
}