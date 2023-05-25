#include <stdio.h>
#include <stdlib.h>
#include "active_object.h"

void* run(void* arg) {
    ActiveObject* ao = (ActiveObject*)arg;
    void* task;
    while ((task = queue_pop(ao->queue)) != NULL) { // Loop will break on NULL task
        ao->func(task);
    }
    return NULL;
}

ActiveObject* createActiveObject(int (*func)(void*)) {
    ActiveObject* ao = (ActiveObject*)malloc(sizeof(ActiveObject));
    if (!ao) {
        return NULL;
    }
    ao->queue = queue_init();
    ao->func = func;
    ao->active = 1;
    if (pthread_create(&ao->thread, NULL, run, ao) != 0) {
        free(ao);
        return NULL;
    }
    return ao;
}

Queue* getQueue(ActiveObject* ao) {
    return ao->queue;
}

void stopActiveObject(ActiveObject* ao) {
    ao->active = 0;
    pthread_join(ao->thread, NULL);
    queue_destroy(ao->queue);
    free(ao);
}