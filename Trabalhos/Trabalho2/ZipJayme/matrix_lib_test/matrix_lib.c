#include <stdlib.h>
#include "matrix_lib.h"
#include <immintrin.h>

int scalar_matrix_mult(float scalar_value, struct matrix *matrix) {
    if (matrix == NULL)
        return 0;

    unsigned long int h = matrix->height;
    unsigned long int w = matrix->width;
    if (h == 0 || w == 0)
        return 0;

    unsigned long int size = h * w;
    float *data = matrix->rows;

    __m256 scalar_vec = _mm256_set1_ps(scalar_value);

    unsigned long int i = 0;
    for (; i + 8 <= size; i += 8) {
        __m256 vals = _mm256_loadu_ps(&data[i]);
        __m256 result = _mm256_mul_ps(vals, scalar_vec);
        _mm256_storeu_ps(&data[i], result);
    }

    // (se size não é múltiplo de 8)
    for (; i < size; i++) {
        data[i] *= scalar_value;
    }

    return 1;
}

int matrix_matrix_mult(struct matrix *matrixA, struct matrix *matrixB, struct matrix *matrixC) {
    if (matrixA == NULL || matrixB == NULL || matrixC == NULL)
        return 0;

    unsigned long int hA = matrixA->height;
    unsigned long int wA = matrixA->width;
    unsigned long int hB = matrixB->height;
    unsigned long int wB = matrixB->width;
    unsigned long int hC = matrixC->height;
    unsigned long int wC = matrixC->width;

    if (hA == 0 || wA == 0 || hB == 0 || wB == 0 || hC == 0 || wC == 0)
        return 0;

    if (wA != hB || hC != hA || wC != wB)
        return 0;

    float *A = matrixA->rows;
    float *B = matrixB->rows;
    float *C = matrixC->rows;

    for (unsigned long int i = 0; i < hA; i++) {    
        for (unsigned long int n = 0; n < wA; n++) {
            float a_in = A[i * wA + n];
            __m256 a_vec = _mm256_set1_ps(a_in);

            unsigned long int p = 0;

            // blocos de 8 floats
            for (; p + 8 <= wB; p += 8) {
                __m256 b_vec = _mm256_loadu_ps(&B[n * wB + p]);
                __m256 c_vec = _mm256_loadu_ps(&C[i * wC + p]);
                __m256 result = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
                _mm256_storeu_ps(&C[i * wC + p], result);
            }

            //(se wB não múltiplo de 8)
            for (; p < wB; p++) {
                C[i * wC + p] += a_in * B[n * wB + p];
            }
        }
    }

    return 1;
}
