#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include <pthread.h>
#include "queue.h"

typedef struct {
    Queue* queue;
    pthread_t thread;
    int (*func)(void*);  // Function to call for each queue item.
    int active;
} ActiveObject;

ActiveObject* createActiveObject(int (*func)(void*));
Queue* getQueue(ActiveObject* ao);
void stopActiveObject(ActiveObject* ao);

#endif //ACTIVEOBJECT_H