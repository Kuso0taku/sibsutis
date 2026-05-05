#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "fioqueue.h"

// test queue
static Queue* create_test_queue() {
    Queue *q = queue_create();
    if (!q) return NULL;

    size_t r0 = 2, c0 = 3, r1 = 1, c1 = 4, r2 = 3, c2 = 1;
    double v0[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    double v1[] = {10.0, 20.0, 30.0, 40.0};
    double v2[] = {100.0, 200.0, 300.0};

    // matrix 0
    Matrix2D *m = matrix2d_construct_default();
    matrix2d_construct(m, r0, c0, NULL);
    m->data = (double*)malloc(r0 * c0 * sizeof(double));
    assert(m->data);
    memcpy(m->data, v0, r0 * c0 * sizeof(double));
    enqueue(q, m);

    // matrix 1
    m = matrix2d_construct_default();
    matrix2d_construct(m, r1, c1, NULL);
    m->data = (double*)malloc(r1 * c1 * sizeof(double));
    assert(m->data);
    memcpy(m->data, v1, r1 * c1 * sizeof(double));
    enqueue(q, m);

    // matrix 2
    m = matrix2d_construct_default();
    matrix2d_construct(m, r2, c2, NULL);
    m->data = (double*)malloc(r2 * c2 * sizeof(double));
    assert(m->data);
    memcpy(m->data, v2, r2 * c2 * sizeof(double));
    enqueue(q, m);

    return q;
}

// save/load text
void test_save_load_text(const char *file) {
    printf("Test: save/load text...\n");
    Queue *q = create_test_queue();
    assert(q != NULL);
    assert(queue_size(q) == 3);

    int rc = save_text_queue_to(q, file);
    assert(rc == 0);

    Queue *q2 = queue_create();
    assert(q2 != NULL);
    rc = load_text_queue_from(q2, file);
    assert(rc == 0);
    assert(queue_size(q2) == 3);

    Iterator it;
    iter_begin(&it, q2);
    Matrix2D *m1 = (Matrix2D*)iter_get(&it);
    assert(m1->rows == 2);
    assert(m1->cols == 3);
    assert(*(m1->data) == 1.0);

    free_memory(q);
    free_memory(q2);
    printf("  PASS\n");
}

// get element valid
void test_get_text_element_valid(const char *file) {
    printf("Test: get_text_element valid index...\n");
    Queue *q = create_test_queue();
    save_text_queue_to(q, file);
    free_memory(q);

    Matrix2D *m = get_text_element(file, 1);
    assert(m != NULL);
    assert(m->rows == 1);
    assert(m->cols == 4);
    assert(*(m->data) == 10.0);
    assert(*(m->data + 3) == 40.0);

    matrix2d_destruct(m);
    printf("  PASS\n");
}

// get element out of range
void test_get_text_element_invalid(const char *file) {
    printf("Test: get_text_element out-of-range...\n");
    Queue *q = create_test_queue();
    save_text_queue_to(q, file);
    free_memory(q);

    Matrix2D *m = get_text_element(file, 10);
    assert(m == NULL);

    printf("  PASS (expected NULL)\n");
}

// binary save/load
void test_binary_save_load(const char *binfile) {
    printf("Test: save/load binary...\n");
    Queue *q = create_test_queue();
    int rc = save_binary_queue_to(q, binfile);
    assert(rc == 0);

    Queue *q2 = queue_create();
    rc = load_binary_queue_from(q2, binfile);
    assert(rc == 0);
    assert(queue_size(q2) == 3);

    Matrix2D *m = NULL;
    rc = get_binary_element(binfile, 0, &m);
    assert(rc == 0);
    assert(m != NULL);
    assert(m->rows == 2 && m->cols == 3);
    matrix2d_destruct(m);

    rc = get_binary_element(binfile, 2, &m);
    assert(rc == 0);
    assert(m->rows == 3 && m->cols == 1);
    matrix2d_destruct(m);

    free_memory(q);
    free_memory(q2);
    printf("  PASS\n");
}

// free memory no crash
void test_free_memory(void) {
    printf("Test: free_memory...\n");
    Queue *q = queue_create();
    Matrix2D *m = matrix2d_construct_default();
    matrix2d_construct(m, 2, 2, NULL);
    m->data = (double*)malloc(4 * sizeof(double));
    *m->data = 1.0;
    enqueue(q, m);
    assert(queue_size(q) == 1);
    free_memory(q);
    printf("  PASS (no crash)\n");
}

// fail test
void test_intentional_fail(void) {
    printf("Test: intentional fail...\n");
    assert(1 == 2);
}

int main(void) {
    const char *test_txt = "test.txt";
    const char *test_bin = "test.bin";

    printf("=== UNIT TESTS START ===\n\n");

    test_save_load_text(test_txt);
    test_get_text_element_valid(test_txt);
    test_get_text_element_invalid(test_txt);
    test_binary_save_load(test_bin);
    test_free_memory();

    //test_intentional_fail();

    printf("\n=== ALL TESTS PASSED ===\n");
    return 0;
}
