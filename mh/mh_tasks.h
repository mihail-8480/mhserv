#ifndef MHSERV_MH_TASKS_H
#define MHSERV_MH_TASKS_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mh_destructor.h"

// Fucking bullshit :)
typedef void* mh_task_result_t;

// More bullshit
typedef void* mh_task_args_t;

// An action that is executed by a task
typedef mh_task_result_t (*action_t)(mh_task_args_t);

// The task structure
typedef struct mh_task {
    mh_destructor_t destructor;
    void* args;
    void* result;
    action_t action;
    pthread_t thread;
    bool destroy_result;
} mh_task_t;

// Even more bullshit
typedef mh_task_t* mh_task;

// Create a task
mh_task mh_task_create(action_t action, mh_task_args_t args, bool destroy_result);
// Run a task
void mh_task_run(mh_task mh_task);
// Wait for a task to finish
mh_task_result_t mh_task_wait(mh_task mh_task);

// Have a task return something
mh_task_result_t mh_task_return(void* value, size_t size);

#endif //MHSERV_MH_TASKS_H