#include "queue.h"


void queue_push(queue_t* q, u8 e)
{
    if (q->size < QUEUE_MAX_SIZE)
    {
        q->elements[q->last] = e;
        q->last = (u8)(q->last + 1) & (QUEUE_MAX_SIZE - 1);
        q->size++;
    }
}


void queue_pop(queue_t* q, u8* e)
{
    if (q->size > 0)
    {
        *e = q->elements[q->first];
        q->first = (u8)(q->first + 1) & (QUEUE_MAX_SIZE - 1);
        q->size--;
    }
}
