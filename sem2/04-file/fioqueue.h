#ifndef FIOQUEUE_H 
#define FIOQUEUE_H 

#include <03-queue/contqueue.h>
#include <01-struct/matrix2d.h>
#include <stddef.h> // size_t

// Grade "Satisfactory"
Matrix2d* rand_gen_matrix2d(size_t);
int save_queue_to(const Queue*, const char*);
int load_queue_from(Queue*, const char*);
Matrix2d* get_text_element(const char*, int); // get by index

// Grade "Good"
int saveb_queue_to(const Queue*, const char*);
int loadb_queue_from(Queue*, const char*);
int get_binary_element(const char*, int, size_t, void*); // get by index

// Grade "Excellent"
Queue* rand_gen_matrix2d_in_queue(size_t);

#endif
