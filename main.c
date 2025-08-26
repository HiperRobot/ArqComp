#include <stdio.h>
#include <stdlib.h>
#include "matrix_lib.h"

// Function to create a matrix
struct matrix* create_matrix(unsigned long int height, unsigned long int width) {
    struct matrix* m = (struct matrix*)malloc(sizeof(struct matrix));
    m->height = height;
    m->width = width;
    m->rows = (float*)calloc(height * width, sizeof(float));
    return m;
}

// Function to print a matrix
void print_matrix(struct matrix* m) {
    for (unsigned long int i = 0; i < m->height; i++) {
        for (unsigned long int j = 0; j < m->width; j++) {
            printf("%.2f ", m->rows[i * m->width + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Test 1: Scalar multiplication
    printf("Test 1: Scalar multiplication\n");
    struct matrix* m1 = create_matrix(2, 2);
    m1->rows[0] = 1.0; m1->rows[1] = 2.0;
    m1->rows[2] = 3.0; m1->rows[3] = 4.0;
    
    printf("Original matrix:\n");
    print_matrix(m1);
    
    float scalar = 2.0;
    scalar_matrix_mult(scalar, m1);
    
    printf("After multiplication by %f:\n", scalar);
    print_matrix(m1);

    // Test 2: Matrix multiplication
    printf("Test 2: Matrix multiplication\n");
    struct matrix* a = create_matrix(2, 3);
    struct matrix* b = create_matrix(3, 2);
    struct matrix* c = create_matrix(2, 2);

    // Initialize matrix A
    a->rows[0] = 1.0; a->rows[1] = 2.0; a->rows[2] = 3.0;
    a->rows[3] = 4.0; a->rows[4] = 5.0; a->rows[5] = 6.0;

    // Initialize matrix B
    b->rows[0] = 1.0; b->rows[1] = 2.0;
    b->rows[2] = 3.0; b->rows[3] = 4.0;
    b->rows[4] = 5.0; b->rows[5] = 6.0;

    printf("Matrix A:\n");
    print_matrix(a);
    printf("Matrix B:\n");
    print_matrix(b);

    matrix_matrix_mult(a, b, c);

    printf("Result of A * B:\n");
    print_matrix(c);

    // Free memory
    free(m1->rows); free(m1);
    free(a->rows); free(a);
    free(b->rows); free(b);
    free(c->rows); free(c);

    return 0;
}
