#include "queue.h"

Queue* queue_init() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->cond, NULL);
    return q;
}

void queue_push(Queue* q, void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    new_node->data = data;
    new_node->next = NULL;
    pthread_mutex_lock(&q->lock);
    if (q->tail != NULL) {
        q->tail->next = new_node;
    }
    q->tail = new_node;
    if (q->head == NULL) {
        q->head = new_node;
    }
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->lock);
}

void* queue_pop(Queue* q) {
    pthread_mutex_lock(&q->lock);
    while (q->head == NULL) {
        pthread_cond_wait(&q->cond, &q->lock);
    }
    Node* old_head = q->head;
    void* data = old_head->data;
    q->head = old_head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    free(old_head);
    pthread_mutex_unlock(&q->lock);
    return data;
}

void queue_destroy(Queue* q) {
    while (q->head != NULL) {
        Node* old_head = q->head;
        q->head = old_head->next;
        free(old_head);
    }
    pthread_cond_destroy(&q->cond);
    pthread_mutex_destroy(&q->lock);
    free(q);
}