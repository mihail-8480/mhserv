#include "include.h"
#ifndef MHSERV_MH_TASKS
#define MHSERV_MH_TASKS

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

static mh_task_args_t mh_task_runner(mh_task_args_t ptr) {
    mh_task self = (mh_task)ptr;
    self->result = self->action(self->args);
    if (self->destroy_result && self->result != NULL)
    {
        free(self->result);
        self->result = NULL;
        free(ptr);
        return NULL;
    }
    return self->result;
}

mh_task mh_task_create(action_t action, mh_task_args_t args, bool destroy_result) {
    mh_task mh_task = calloc(1,sizeof(mh_task_t));
    *mh_task = (mh_task_t){
            .args = args,
            .action = action,
            .destroy_result = destroy_result
    };
    mh_task->result = NULL;
    return mh_task;
}

void mh_task_run(mh_task mh_task) {
    pthread_create(&mh_task->thread, NULL, mh_task_runner, mh_task);
}
/*
mh_task_result_t mh_task_wait(mh_task mh_task) {
    void *result;
    pthread_join(mh_task->thread, &result);
    return result;
}

mh_task_result_t mh_task_return(mh_task_any_t value, size_t size) {
    mh_task_result_t result = malloc(size);
    memcpy(result, value, size);
    return result;
}

void mh_task_destroy(mh_task mh_task) {
    if (mh_task->result != NULL)
        free(mh_task->result);
    free(mh_task);
}*/

#endif //MHSERV_MH_TASKS
