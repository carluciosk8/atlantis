#pragma once

#include "msxgl.h"


#define QUEUE_MAX_SIZE 32

typedef struct
{
    u8 elements[QUEUE_MAX_SIZE];
    u8 first;
    u8 last;
    u8 size;
} queue_t;


#define queue_init(q)       q.first = 0 ; q.last = 0 ; q.size = 0;
#define queue_is_empty(q)   (q.first == q.last && q.size == 0)

void queue_push(queue_t* q, u8 e);
void queue_pop(queue_t* q, u8* s);
