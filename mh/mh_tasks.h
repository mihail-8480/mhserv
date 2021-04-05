#ifndef MHSERV_MH_TASKS_H
#define MHSERV_MH_TASKS_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef void* mh_task_result_t;
typedef void* mh_task_args_t;
typedef mh_task_result_t (*action_t)(mh_task_args_t);

typedef struct mh_task {
    void* args;
    void* result;
    action_t action;
    pthread_t thread;
    bool destroy_result;
} mh_task_t;

typedef mh_task_t* mh_task;
mh_task_args_t mh_task_runner(mh_task_args_t ptr);
mh_task mh_task_create(action_t action, mh_task_args_t args, bool destroy_result);
void mh_task_run(mh_task mh_task);
mh_task_result_t mh_task_wait(mh_task mh_task);
mh_task_result_t mh_task_return(void* value, size_t size);
void mh_task_destroy(mh_task mh_task);

#endif //MHSERV_MH_TASKS_H