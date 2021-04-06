#ifndef MHSERV_MH_TASKS_H
#define MHSERV_MH_TASKS_H

#include "mh_destructor.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// An action that is executed by a task
typedef void* (*action_t)(void*);

// The task structure
typedef struct mh_task {
    mh_destructor_t destructor;
    void* args;
    void* result;
    action_t action;
    pthread_t thread;
    bool destroy_result;
} mh_task_t;

// Create a task
mh_task_t* mh_task_create(action_t action, void* args, bool destroy_result);
// Run a task
void mh_task_run(mh_task_t* mh_task);
// Wait for a task to finish
void* mh_task_wait(mh_task_t* mh_task);

// Have a task return something
void* mh_task_return(void* value, size_t size);

#endif //MHSERV_MH_TASKS_H