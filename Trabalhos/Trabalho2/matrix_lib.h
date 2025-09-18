#ifndef MATRIX_LIB_H
#define MATRIX_LIB_H

#include <stddef.h>

void scalar_matrix_mult(float scalar, const float *A, float *R, int N);
void matrix_matrix_mult(const float *A, const float *B, float *C, int N);

#endif // MATRIX_LIB_H
