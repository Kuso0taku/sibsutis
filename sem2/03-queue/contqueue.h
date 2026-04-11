#ifndef CONTQUEUE_H
#define CONTQUEUE_H

#include <stddef.h> // size_t

// queue node
typedef struct Node {
  void* data;
  struct Node* next;
} Node;

// queue structure
typedef struct {
  Node* front;      // first element 
  Node* rear;       // last element 
  size_t size;      // nubmer of elements 
  size_t capacity;  // all alocated nodes 
  Node* pool;       // free nodes stack 
} Queue;

// iterator structure
typedef struct {
  const Queue* queue; // points at this queue
  Node* current;      // current node
} Iterator;


// Grade "Satisfactory"
Queue* queue_create(void); // alloc memory
void queue_free(Queue* q); // free memory
                           
void enqueue(Queue* q, void* data); // add to queue 
void* dequeue(Queue* q); // get from queue and remove

// Grade "Good"
_Bool queue_empty(const Queue* q); // true if queue is empty
size_t queue_size(const Queue* q); // get size
void* queue_peek(const Queue* q); // check the first from queue 
void queue_clear(Queue* q); // clear queue 

// Grade "Excellent"
void frenqueue(Queue* q, void* data); // add to the beggining
Queue* queue_copy(Queue* dst, const Queue* src); // copy queue 
Queue* queue_merge(Queue* dst, Queue* src); // merge queues into one

// Iterator 
void iter_begin(Iterator* iter, const Queue* q);
void iter_next(Iterator* iter);
void iter_end(Iterator* iter, const Queue* q);

void* iter_get(const Iterator* iter); // get current element
_Bool iter_belongs_at(const Iterator* iter, const Queue* q); //if iterator points at q
_Bool iter_equal(const Iterator* iter1, const Iterator* iter2); // if iterators equel

#endif
