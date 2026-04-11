#include <stdio.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include "contqueue.h"

#define INT_MIN -100 
#define INT_MAX 100

// generate random number
int getrand_int(int min, int max) {
  return (int)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

// printf queue w/o delete it
void print_int_queue(Queue* q) {
  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);
  
  while (!iter_equal(&cur, &end)) {
    printf("%d  ", *(int*)iter_get(&cur));
    iter_next(&cur);
  }
  putchar('\n');
}

int main() {
  srand(time(0));
  size_t n = 10;
  
  printf("INT QUEUE\n\n");

  int* nums = (int*)malloc(sizeof(int)*n);
  for (size_t i=0; i<n; i++) *(nums+i) = getrand_int(INT_MIN, INT_MAX);
  
  printf("data: ");
  for (size_t i=0; i<n-1; i++) printf("%d, ", *(nums+i));
  printf("%d\n\n", *(nums+n-1));

  Queue* q_int = queue_create();
  for (size_t i=0; i<n/2; i++) {
    enqueue(q_int, nums+i);
    frenqueue(q_int, nums+n-1-i);
  }

  printf("size = %zu, last element = %d\n", 
      queue_size(q_int), *(int*)queue_peek(q_int));
  printf("element %d has been removed, ", *(int*)dequeue(q_int));
  printf("size now = %zu\n", queue_size(q_int));
  
  printf("queue:\n");
  print_int_queue(q_int);

  Queue* q_int_cp = queue_create();
  queue_copy(q_int_cp, q_int);
  printf("\nqueue copy:\n");
  print_int_queue(q_int_cp);

  printf("\nmerge original queue and copy\n");
  queue_merge(q_int, q_int_cp);
  printf("result:\n");
  print_int_queue(q_int);
  
  // free memory
  queue_clear(q_int_cp);
  queue_clear(q_int);
  free(nums);

  return 0;
}
