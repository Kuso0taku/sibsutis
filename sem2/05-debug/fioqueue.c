#include "debug.h"
#include "fioqueue.h"
#include <stdlib.h>
#include <stdio.h>

// additional
static size_t gen_rand_size_t(size_t min, size_t max) {
  return (size_t)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

static void fill_rand_matrix2d(Matrix2D* m) {
    size_t rows = gen_rand_size_t(1, MAX_ROW);
    size_t cols = gen_rand_size_t(1, MAX_COL);
    
    matrix2d_construct(m, rows, cols, NULL);

    m->data = (double*)malloc(rows*cols* sizeof(double)); // alloc memory for data
    if (!m->data) {
      fprintf(stderr, "Memory allocation failed.\n");
      exit(1);
    }
    LOG("allocated %zu bytes at %p", rows*cols*sizeof(double), (void*)m->data);
    
    matrix2d_random(m, MIN_DOUBLE, MAX_DOUBLE);
}

// Grade "Satisfactory"
Matrix2D* rand_gen_matrix2d(size_t n) {
  Matrix2D* arr = (Matrix2D*)malloc(n * sizeof(Matrix2D));
    LOG("allocated %zu bytes at %p", n*sizeof(Matrix2D), (void*)arr);

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
    Matrix2D* m = (Matrix2D*)iter_get(&cur);
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
    Matrix2D* m = matrix2d_construct_default();
    matrix2d_construct(m, rows, cols, NULL);

    m->data = (double*)malloc(rows*cols* sizeof(double)); // alloc memory for data
    if (!m->data) {
      fprintf(stderr, "Memory allocation failed.\n");
      exit(1);
    }
    LOG("allocated %zu bytes at %p", rows*cols*sizeof(double), (void*)m->data);

    for (size_t i=0; i < rows*cols; i++)
      if (fscanf(f, "%lf", m->data+i) != 1) {
        matrix2d_destruct(m);
      LOG("error path: destruct matrix %p (data=%p)", 
          (void*)m, (void*)m->data);
        break;
      }
    
    if (m) enqueue(q, m);
  }

  fclose(f);
  return 0;
}

Matrix2D* get_text_element(const char* filename, int index) {
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

  Matrix2D* m = matrix2d_construct_default();
  matrix2d_construct(m, rows, cols, NULL);
  m->data = (double*)malloc(rows*cols* sizeof(double)); // alloc memory for data
  if (!m->data) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }
    LOG("allocated %zu bytes at %p", rows*cols*sizeof(double), (void*)m->data);

  for (size_t i=0; i < rows*cols; i++) 
    if (fscanf(f, "%lf", m->data+i) != 1) {
      LOG("freeing matrix %p: rows=%zu, cols=%zu, data=%p", 
          (void*)m, m->rows, m->cols, (void*)m->data);
      matrix2d_destruct(m);
      LOG("matrix %p destructed", (void*)m);
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
    Matrix2D* m = (Matrix2D*)iter_get(&cur);
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
    
    Matrix2D* m = matrix2d_construct_default();
    matrix2d_construct(m, rows, cols, NULL);

    m->data = (double*)malloc(rows*cols* sizeof(double)); // alloc memory for data
    if (!m->data) {
      fprintf(stderr, "Memory allocation failed.\n");
      exit(1);
    }
    LOG("allocated %zu bytes at %p", rows*cols*sizeof(double), (void*)m->data);

    if (fread(m->data, sizeof(double), m->rows * m->cols, f) != rows*cols) {
      LOG("freeing matrix %p: data=%p", (void*)m, (void*)m->data);
      matrix2d_destruct(m);
      LOG("matrix %p destructed", (void*)m);
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
  for (int i=0; i < index; i++) {
    if (fread(&r, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }
    if (fread(&c, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }

    fseek(f, (long)(r*c * sizeof(double)), SEEK_CUR);
  }
  
  // read matrix
  if (fread(&r, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }
  if (fread(&c, sizeof(size_t), 1, f) != 1) { fclose(f); return -1; }

  Matrix2D* m = matrix2d_construct_default();
  matrix2d_construct(m, r, c, NULL);

  m->data = (double*)malloc(r*c* sizeof(double)); // alloc memory for data
  if (!m->data) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }
  LOG("allocated %zu bytes at %p", r*c*sizeof(double), (void*)m->data);

  
  if (fread(m->data, sizeof(double), r*c, f) != r*c) {
    LOG("freeing matrix %p: data=%p", (void*)m, (void*)m->data);
    matrix2d_destruct(m);
    LOG("matrix %p destructed", (void*)m);
    fclose(f);
    return -1;
  }
  *(Matrix2D**)result = m;

  fclose(f);
  return 0;
}

// Grade "Excellent"
Queue* rand_gen_matrix2d_in_queue(size_t n) {
  Queue* q = queue_create();

  for (size_t i=0; i<n; i++) {
    Matrix2D* m = matrix2d_construct_default();

    fill_rand_matrix2d(m);

    enqueue(q, m);
  }

  return q;
}

// Also 
int list_text(const char* filename) {
  Queue* q = queue_create();
  if (load_text_queue_from(q, filename) != 0) {
    free_memory(q);
    return -1;
  }

  size_t size = queue_size(q);
  printf("Total elements: %zu\n", size);

  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);

  size_t idx=0;

  while (!iter_equal(&cur, &end)) {
    Matrix2D* m = (Matrix2D*)iter_get(&cur);

    printf("\n[%zu] rows=%zu, cols=%zu\nElements:\n", idx, m->rows, m->cols);
    for (size_t i=0; i < m->rows * m->cols; i++) printf(" %.2f", *(m->data+i));
    putchar('\n');

    iter_next(&cur);
    idx++;
  }

  free_memory(q);
  return 0;
}

int list_binary(const char* filename) {
  Queue* q = queue_create();
  if (load_binary_queue_from(q, filename) != 0) {
    free_memory(q);
    return -1;
  }

  size_t size = queue_size(q);
  printf("Total elements: %zu\n", size);

  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);

  size_t idx=0;

  while (!iter_equal(&cur, &end)) {
    Matrix2D* m = (Matrix2D*)iter_get(&cur);

    printf("\n[%zu] rows=%zu, cols=%zu\nElements:\n", idx, m->rows, m->cols);
    for (size_t i=0; i < m->rows * m->cols; i++) printf("  %.2f", *(m->data+i));
    putchar('\n');

    iter_next(&cur);
    idx++;
  }

  free_memory(q);
  return 0;
}

// free queue and every matrix memory
void free_memory(Queue* q) {
  if (!q) return;
  LOG("start freeing queue %p", (void*)q);
  Iterator cur, end;
  iter_begin(&cur, q);
  iter_end(&end, q);
  
  // free every matrix memory
  while (!iter_equal(&cur, &end)) {
    Matrix2D* m = (Matrix2D*)iter_get(&cur);
    if (m) {
      LOG("freeing matrix %p: rows=%zu, cols=%zu, data=%p", 
          (void*)m, m->rows, m->cols, (void*)m->data);
      matrix2d_destruct(m);
      LOG("matrix %p destructed", (void*)m);
      cur.current->data = NULL;
    }
    iter_next(&cur);
  }
  LOG("all matrices freed, freeing queue container");
  queue_free(q);
  LOG("queue %p freed completely", (void*)q);
}
