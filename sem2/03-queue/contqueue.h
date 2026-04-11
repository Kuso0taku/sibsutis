#ifndef CONTQUEUE_H
#define CONTQUEUE_H

#include <stddef.h> // size_t

typedef struct Queue Queue;
typedef struct Iterator Iterator;

// Grade "Satisfactory"
Queue* create_queue(void); // alloc memory
void queue_free(Queue* q); // free memory
                           
void enqueue(Queue* q, void* data); // add to queue 
void* dequeue(Queue* q); // get from queue and remove

// Grade "Good"
_Bool queue_empty(const Queue* q); // true if queue is empty
size_t queue_size(const Queue* q); // get size
void* queue_peek(const Qeueu* q); // check the first from queue 
void queue_clear(Queue* q); // clear queue 

// Grade "Excellent"
void frenqueue(Queue* q, void* data); // add to the beggining
Queue* queue_copy(Queue* dst, const Queue* src); // copy queue 
Queue* queue_merge(Queue* dst, const Queue* src); // merge queues into one

// Iterator 
Iterator* iter_begin(const Queue* q);
Iterator* iter_next(Iterator* iter);
Iterator* iter_end(const Queue* q);

void* iter_get(const Iterator* it); // get current element
_Bool iter_belongs_at(const Iterator* iter, const Queue* q); //if iterator points at q
_Bool iter_equal(const Iterator* iter1, const Iterator* iter2); // if iterators equel

#endif
