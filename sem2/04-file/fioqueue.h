#ifndef FIOQUEUE_H 
#define FIOQUEUE_H 

#include <03-queue/contqueue.h>
#include <01-struct/matrix2d.h>
#include <stddef.h> // size_t

// for generators
#define MAX_DOUBLE 10000
#define MIN_DOUBLE -10000

#define MAX_ROW 100
#define MAX_COL 100 

// Grade "Satisfactory"
Matrix2D* rand_gen_matrix2d(size_t n);
int save_text_queue_to(const Queue* q, const char* filename);
int load_text_queue_from(Queue* q, const char* filename);
Matrix2D* get_text_element(const char* filename, int index); // get by index

// Grade "Good"
int save_binary_queue_to(const Queue* q, const char* filename);
int load_binary_queue_from(Queue* q, const char* filename);
int get_binary_element(const char* filename, int index, void* result); // get by index

// Grade "Excellent"
Queue* rand_gen_matrix2d_in_queue(size_t n);

// Also 
int list_text(const char* filename);
int list_binary(const char* filename);

#endif
