#ifndef MHSERV_MH_THREAD_H
#define MHSERV_MH_THREAD_H

// Code that needs to get executed on a new thread
typedef void *(*mh_thread_action_t)(void *);

// Do an action on a new thread
void mh_thread_create(mh_thread_action_t action, void *args);

// Exit from the current thread
_Noreturn void mh_thread_exit(void *ret);

#endif //MHSERV_MH_THREAD_H
