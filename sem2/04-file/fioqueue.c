#include "fioqueue.h"
#include <time.h>
#include <stdlib.h>

// additional
static size_t gen_rand_size_t(size_t min, size_t max) {
  return (size_t)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

static void fill_rand_matrix2d(Matrix2d* m) {
    size_t rows = gen_rand_size_t(1, MAX_ROW);
    size_t cols = gen_rand_size_t(1, MAX_COL);
  
    matrix2d_construct(m, rows, cols, NULL);
    matrix2d_random(m, MIN_DOUBLE, MAX_DOUBLE);
}

// Grade "Satisfactory"
Matrix2d* rand_gen_matrix2d(size_t n) {
  Matrix2d* arr = (Matrix2d*)malloc(n * sizeof(Matrix2d));

  for (size_t i=0; i<n; i++) {
    fill_rand_matrix2d(arr+i);
  }

  return arr;
}

int save_text_queue_to(const Queue* q, const char* filename) {
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

int load_text_queue_from(Queue* q, const char* filename) {
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
int save_binary_queue_to(const Queue* q, const char* filename) {
  FILE* f = fopen(filename, "wb");
  if (!f) return -1;

  size_t q_size = queue_size(q);
  fwrite(&q_size, sizeof(size_t), 1, f);

  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);
  
  while (!iter_equal(&cur, &end)) {
    Matrix2d* m = (Matrix2d*)iter_get(&cur);
    fwrite(&m->rows, sizeof(size_t), 1, f);
    fwrite(&m->cols, sizeof(size_t), 1, f);
  
    fwrite(m->data, sizeof(double), m->rows * m->cols, f);

    iter_next(&cur);
  }

  fclose(f);
  return 0;
}

int load_binary_queue_from(Queue* q, const char* filename) {
  FILE* f = fopen(filename, "rb");
  if (!f) return -1;

  size_t q_size;
  fread(&q_size, sizeof(size_t), 1, f);
  
  size_t rows, cols;
  for (size_t i=0; i < q_size; i++) {
    if (fread(&rows, sizeof(size_t), 1, f) != 1) break;
    if (fread(&cols, sizeof(size_t), 1, f) != 1) break;
    
    Matrix2d* m = matrix2d_construct_default();
    matrix2d_construct(m, rows, cols, NULL);

    if (fread(m->data, sizeof(double), m->rows * m->cols, f) != rows*cols) {
      matrix2d_destruct(m);
      break;
    }
    
    if (m) enqueue(q, m);
  }

  fclose(f);
  return 0;

}

int get_binary_element(const char* filename, int index, void* result) {
  FILE* f = fopen(filename, "rb");
  if (!f) return -1;

  size_t q_size;
  if (fread(&q_size, sizeof(size_t), 1, f) != 1) {
    fclose(f);
    return -1;
  }

  if (index < 0 || (size_t)index >= q_size) {
    fclose(f);
    return -2;
  }
  
  size_t r, c;

  // skip matrices
  for (size_t i=0; i < index; i++) {
    if (fread(&r, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }
    if (fread(&c, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }

    fseek(f, (long)(r*c * sizeof(double)), SEEK_CUR);
  }
  
  // read matrix
  if (fread(&r, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }
  if (fread(&c, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }

  Matrix2d* m = matrix2d_construct_default();
  matrix2d_construct(m, r, c, NULL);
  
  if (fread(m->data, sizeof(double), r*c, f) != r*c) {
    matrix2d_destruct(m);
    fclose(f);
    return -1;
  }
  *(Matrix2d**)result = m;

  fclose(f);
  return 0;
}

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

// Also 
int list_text(const char* filename) {
  Queue* q = queue_create();
  if (load_text_queue_from(q, filename) != 0) {
    queue_free(q);
    return -1;
  }

  size_t size = queue_size(q);
  printf("Total elements: %zu\n", size);

  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);

  size_t idx=0;

  while (!iter_equal(&cur, &end)) {
    Matrix2d* m = (Matrix2d*)iter_get(&cur);

    printf("[%zu] rows=%zu, cols=%zu\nElements:\n", idx, m->rows, m->cols);
    for (size_t i=0; i < m->rows * m->cols; i++) printf(" %.2f", *(m->data+i));
    putchar('\n');

    iter_next(&cur);
    idx++;
  }

  queue_free(q);
  return 0;
}

int list_binary(const char* filename) {
  Queue* q = queue_create();
  if (load_binary_queue_from(q, filename) != 0) {
    queue_free(q);
    return -1;
  }

  size_t size = queue_size(q);
  printf("Total elements: %zu\n", size);

  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);

  size_t idx=0;

  while (!iter_equal(&cur, &end)) {
    Matrix2d* m = (Matrix2d*)iter_get(&cur);

    printf("[%zu] rows=%zu, cols=%zu\nElements:\n", idx, m->rows, m->cols);
    for (size_t i=0; i < m->rows * m->cols; i++) printf("  %.2f", *(m->data+i));
    putchar('\n');

    iter_next(&cur);
    idx++;
  }

  queue_free(q);
  return 0;
}
