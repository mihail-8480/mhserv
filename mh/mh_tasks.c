#include "mh_tasks.h"

void mh_task_free(void * ptr) {
    mh_task_t* mh_task = (mh_task_t*)ptr;
    // Destroy the used memory of a task
    if (mh_task->result != NULL)
        free(mh_task->result);
    free(mh_task);
}

void* mh_task_return(void *value, size_t size) {
    // Return a value (allocate memory and copy a value to it and get the pointer)
    void* result = malloc(size);
    memcpy(result, value, size);
    return result;
}

void* mh_task_wait(mh_task_t* mh_task) {
    // Wait for a thread/task to finish
    void *result;
    pthread_join(mh_task->thread, &result);
    return result;
}
void* mh_task_runner(void* ptr) {
    // Get the task from the arguments
    mh_task_t* self = (mh_task_t*)ptr;

    // Call the function and set the result
    self->result = self->action(self->args);

    // Hopefully no memory leaks
    if (self->destroy_result && self->result != NULL)
    {
        free(self->result);
        self->result = NULL;
        free(ptr);
        return NULL;
    }
    free(ptr);
    return self->result;
}

void mh_task_run(mh_task_t* mh_task) {
    // Create a new thread
    pthread_create(&mh_task->thread, NULL, mh_task_runner, mh_task);
}

mh_task_t* mh_task_create(action_t action, void* args, bool destroy_result) {
    // Create a new task (i should probably use malloc here)
    mh_task_t* mh_task = calloc(1,sizeof(mh_task_t));
    *mh_task = (mh_task_t){
            .destructor = mh_task_free,
            .args = args,
            .action = action,
            .destroy_result = destroy_result
    };
    mh_task->result = NULL;
    return mh_task;
}

