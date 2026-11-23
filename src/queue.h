#pragma once

#include "msxgl.h"


// Must be a power of two: queue.c wraps the indices with & (QUEUE_MAX_SIZE - 1).
#define QUEUE_MAX_SIZE 32

typedef struct
{
    u8 elements[QUEUE_MAX_SIZE];
    u8 first;
    u8 last;
    u8 size;
} queue_t;


#define queue_init(q)       do { (q).first = 0; (q).last = 0; (q).size = 0; } while (0)

void queue_push(queue_t* q, u8 element);
void queue_pop(queue_t* q, u8* element);
