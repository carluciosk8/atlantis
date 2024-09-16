#include "queue.h"


void queue_push(queue_t* q, uint8_t e)
{
    if (q->size < QUEUE_MAX_SIZE)
    {
        q->elements[q->last] = e;
        q->last = (q->last + 1) % QUEUE_MAX_SIZE;
        q->size++;
    }
}


void queue_pop(queue_t* q, uint8_t* e)
{
    if (q->size > 0)
    {
        *e = q->elements[q->first];
        q->first = (q->first + 1) % QUEUE_MAX_SIZE;
        q->size--;
    }
}
