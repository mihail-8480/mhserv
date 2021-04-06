#ifndef MHSERV_MH_THREAD_H
#define MHSERV_MH_THREAD_H
void mh_thread_create(void*(*callback)(void*), void* args);
#endif //MHSERV_MH_THREAD_H
