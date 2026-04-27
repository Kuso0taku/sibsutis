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

int save_queue_to(const Queue* q, const char* filename) {
  if (!q) return -1;
  FILE *f = fopen(filename, "w");
  if (!f) return -1;
  
  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);
  
  while (!iter_equal(&cur, &end)) {
    Matrix2d* m = (Matrix2d*)iter_get(&cur);
    fprintf(f, "%zu %zu ", m->rows, m->cols);

    for (size_t i=0; i<m->rows * m->cols; i++) fprintf(f, "%.6f ", *(m->data+i));
    
    fprintf(f, "\n");
    iter_next(&cur);
  }

  fclose(f);
  return 0;
}

int load_queue_from(Queue* q, const char* filename) {
  FILE *f = fopen(filename, "r");
  if (!f) return -1;

  size_t rows, cols;
  while (fscanf(f, "%zu %zu", &rows, &cols) == 2) {
    Matrix2d* m = matrix2d_construct_default();
    matrix2d_construct(m, rows, cols, NULL);

    for (size_t i=0; i < rows*cols; i++)
      if (fscanf(f, "%lf", m->data+i) != 1) {
        matrix2d_destruct(m);
        break;
      }
    
    if (m) enqueue(q, m);
  }

  fclose(f);
  return 0;
}

Matrix2d* get_text_element(const char* filename, int index) {
  FILE *f = fopen(filename, "r");
  if (!f) return NULL;
  
  int ch, line = 0;
  while (line < index && (ch = fgetc(f)) != EOF) if (ch == '\n') line++;

  if (line < index) {
    fclose(f);
    return NULL;
  }

  size_t rows, cols;
  if (fscanf(f, "%zu %zu", &rows, &cols) != 2) {
    fclose(f);
    return NULL;
  }

  Matrix2d* m = matrix2d_construct_default();
  matrix2d_construct(m, rows, cols, NULL);

  for (size_t i=0; i < rows*cols; i++) 
    if (fscanf(f, "%lf", m->data+i) != 1) {
      matrix2d_destruct(m);
      fclose(f);
      return NULL;
    }

  fclose(f);
  return m;
}

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
