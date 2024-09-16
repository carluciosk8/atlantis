#pragma once

#include "ubox.h"


#define QUEUE_MAX_SIZE 32

typedef struct
{
    uint8_t elements[QUEUE_MAX_SIZE];
    uint8_t first;
    uint8_t last;
    uint8_t size;
} queue_t;


#define queue_init(q)       q.first = 0 ; q.last = 0 ; q.size = 0;
#define queue_is_empty(q)   (q.first == q.last && q.size == 0)

void queue_push(queue_t* q, uint8_t e);
void queue_pop(queue_t* q, uint8_t* s);
