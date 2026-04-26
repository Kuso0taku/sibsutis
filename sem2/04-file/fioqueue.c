#include "fioqueue.h"
#include <time.h>
#include <stdlib.h>

// additional
static void fill_rand_matrix2d(Matrix2d* m) {
    size_t cols = gen_rand_int(1, MAX);
    size_t rows = gen_rand_int(1, MAX);
  
    matrix2d_construct(m, rows, cols, NULL);
    matrix2d_random(m, MIN, MAX);

    return m
}

// Grade "Satisfactory"
Matrix2d* rand_gen_matrix2d(size_t n) {
  size_t rows, cols;
  Matrix2d* arr = (Matrix2d*)malloc(n * sizeof(Matrix2d));

  for (size_t i=0; i<n; i++) {
    fill_rand_matrix2d(arr+i)
  }

  return arr;
}

int save_queue_to(const Queue*, const char*);
int load_queue_from(Queue*, const char*);
Matrix2d* get_text_element(const char*, int); // get by index

// Grade "Good"
int saveb_queue_to(const Queue*, const char*);
int loadb_queue_from(Queue*, const char*);
int get_binary_element(const char*, int, size_t, void*); // get by index

// Grade "Excellent"
Queue* rand_gen_matrix2d_in_queue(size_t n) {
  Queue* q = queue_create();

  for (size_t i=0; i<n; i++) {
    Matrix2d* m = matrix2d_construct_default();

    fill_rand_matrix2d(m);

    enqueue(q, m);
  }

  return q;
}
