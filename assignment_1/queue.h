/*
* File: queue.h
*/
#include <stdlib.h>

typedef struct queueCDT *queueADT;
typedef struct customerDataT *queueElementT; 

queueADT EmptyQueue(void);
void Enqueue(queueADT queue, queueElementT element);
queueElementT Dequeue(queueADT queue);
int QueueLength(queueADT queue);
int QueueIsEmpty(queueADT queue);