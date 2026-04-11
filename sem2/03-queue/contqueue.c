#include "contqueue.h"
#include <stdlib.h>
#include <math.h> // sqrt

#define INIT_CAPACITY 4 // may be any 
#define GROWTH sqrt(2) // growth factor for memory backup

// some static functions (for internal needs)

// count free nodes
static size_t pool_size(const Queue* q) {
  size_t count = 0;
  Node* cur = q->pool;
  while (cur) {
    count++;
    cur = cur->next;
  }
  return count;
}

// increase capacity and add some nodes to pool 
static void expand_pool(Queue* q) {
  size_t new_cap = (size_t)(q->capacity * GROWTH);
  if (new_cap <= q->capacity) new_cap = q->capacity + 1;
  q->capacity = new_cap;
  
  size_t n = q->capacity - (q->size + pool_size(q)); // how many nodes to add
  for (size_t i=0; i < n; i++) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->next = q->pool;
    q->pool = node;
  }
}

// get node from pool 
static Node* get_node(Queue* q) {
  if (q->pool == NULL) {
    expand_pool(q);
  }
  Node* node = q->pool;
  q->pool = node->next;
  return node;
}

// return node to pool
static void return_node(Queue* q, Node* node) {
  node->next = q->pool;
  q->pool = node;
}

// main functions (public)

// Grade "Satisfactory"

// alloc memory and set initial values
Queue* queue_create(void) {
  Queue* q = (Queue*)malloc(sizeof(Queue));
  if (!q) return NULL;

  q->front = q->rear = NULL;
  q->size = 0;
  q->capacity = INIT_CAPACITY;
  q->pool = NULL;

  for (size_t i=0; i < q->capacity; i++) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->next = q->pool;
    q->pool = node;
  }
  return q;
}

// free memory
void queue_free(Queue* q) {
  if (!q) return;
  
  queue_clear(q);
  Node* cur = q->pool;
  while (cur) {
    Node* next = cur->next;
    free(cur);
    cur = next;
  }
  free(q);
}
  
// add to queue
void enqueue(Queue* q, void* data) {
  Node* node = get_node(q);
  node->data = data;
  node->next = NULL;
  if (q->rear == NULL) q->front = q->rear = node;
  else {
    q->rear->next = node;
    q->rear = node;
  }
  q->size++;
}

// get from queue and remove
void* dequeue(Queue* q) {
  if (q->front == NULL) return NULL;
  Node* temp = q->front;
  void* data = temp->data;
  q->front = q->front->next;
  if (q->front == NULL) q->rear = NULL;
  return_node(q, temp);
  q->size--;
  return data;
}

// true if queue is empty
_Bool queue_empty(const Queue* q) {return q->size == 0;}

// Grade "Good"

// get size
size_t queue_size(const Queue* q) {return q->size;}

// check the first from queue
void* queue_peek(const Queue* q) 
  {return (q->front == NULL) ? NULL : q->front->data;}

// clear queue
void queue_clear(Queue* q) {
  while (q->front != NULL) {
    Node* temp = q->front;
    q->front = q->front->next;
    return_node(q, temp);
  }
  q->rear = NULL;
  q->size = 0;
}

// Grade "Excellent"

// add to the beggining
void frenqueue(Queue* q, void* data) {
  Node* node = get_node(q);
  node->data = data;
  node->next = q->front;
  q->front = node;
  if (q->rear == NULL) q->rear = node;
  q->size++;
}

// copy queue
Queue* queue_copy(Queue* dst, const Queue* src) {
  if (!dst) return NULL;
  queue_clear(dst);

  Node* cur = src->front;
  while (cur) {
    enqueue(dst, cur->data);
    cur = cur->next;
  }
  return dst;
}

// merge queues into one
Queue* queue_merge(Queue* dst, Queue* src) {
  if (!dst || !src) return NULL;
  if (queue_empty(src)) return dst;

  if (queue_empty(dst)) {
    dst->front = src->front;
    dst->rear = src->rear;
    dst->size = src->size;
  } else {
    dst->rear->next = src->front;
    dst->rear = src->rear;
    dst->size += src->size;
  }
  
  src->front = NULL;
  src->rear = NULL;
  src->size = 0;

  return dst;
}

// Iterator 

void iter_begin(Iterator* iter, const Queue* q) {
  iter->queue = q;
  iter->current = (q) ? q->front : NULL;
}

void iter_next(Iterator* iter) {
  if (iter && iter->current)
    iter->current = iter->current->next;
}

void iter_end(Iterator* iter, const Queue* q) {
  iter->queue = q;
  iter->current = NULL;
}

// get current element
void* iter_get(const Iterator* iter) {
  return iter->current ? iter->current->data : NULL;
}

//if iterator points at q
_Bool iter_belongs_at(const Iterator* iter, const Queue* q) {
  return iter->queue == q;
}

// if iterators equal
_Bool iter_equal(const Iterator* iter1, const Iterator* iter2) {
  return (iter1->queue == iter2->queue && iter1->current == iter2->current);
}
