#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "active_object.h"

// Global ActiveObjects
ActiveObject *ao1;
ActiveObject *ao2;
ActiveObject *ao3;
ActiveObject *ao4;

int is_prime(unsigned int num)
{
    if (num <= 1)
        return 0;
    if (num % 2 == 0 && num > 2)
        return 0;
    for (unsigned int i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int task(void *data)
{
    int *num = (int *)data;
    *num = (rand() % 1000000);
    if(*num < 100000)
        *num+= 100000;
    // Pass data to next AO.
    queue_push(getQueue(ao2), num);
    sleep(1);
    return 0;
}

int task2(void *data)
{
    int *num = (int *)data;
    printf("%d\n", *num);
    printf("%s\n", is_prime(*num) ? "true" : "false");
    *num += 11;
    // Pass data to next AO.
    queue_push(getQueue(ao3), num);
    return 0;
}

int task3(void *data)
{
    int *num = (int *)data;
    printf("%d\n", *num);
    printf("%s\n", is_prime(*num) ? "true" : "false");
    *num -= 13;
    // Pass data to next AO.
    queue_push(getQueue(ao4), num);
    return 0;
}

int task4(void *data)
{
    int *num = (int *)data;
    printf("%d\n", *num);
    *num += 2;
    printf("%d\n", *num);
    return 0;
}

int main(int argc, char *argv[])
{
    int N;
    long seed;

    if (argc < 2 || argc > 3)
    {
        printf("Usage: pipeline N [seed]\n");
        return 1;
        printf("\n");
    }
    N = atoi(argv[1]);
    seed = (argc == 3) ? atol(argv[2]) : time(NULL);

    srand(seed);

    // Create the ActiveObjects
    ao1 = createActiveObject(task);
    ao2 = createActiveObject(task2);
    ao3 = createActiveObject(task3);
    ao4 = createActiveObject(task4);

    for (int i = 0; i < N; i++)
    {
        int *num = malloc(sizeof(int)); // Allocating memory for each number.
        queue_push(getQueue(ao1), num);
        sleep(1);
        printf("\n");
    }

    queue_push(getQueue(ao1), NULL); // signal ao1 that no more tasks will be pushed
    queue_push(getQueue(ao2), NULL); // signal ao2 that no more tasks will be pushed
    queue_push(getQueue(ao3), NULL); // signal ao3 that no more tasks will be pushed
    queue_push(getQueue(ao4), NULL); // signal ao4 that no more tasks will be pushed
    stopActiveObject(ao1);
    stopActiveObject(ao2);
    stopActiveObject(ao3);
    stopActiveObject(ao4);

    return 0;
}
