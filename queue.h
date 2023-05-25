#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <pthread.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* head;
    Node* tail;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Queue;

Queue* queue_init();
void queue_push(Queue* q, void* data);
void* queue_pop(Queue* q);
void queue_destroy(Queue* q);

#endif //QUEUE_H