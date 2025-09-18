#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include "matrix_lib.h"

// Aloca matriz
struct matrix *allocate_matrix(unsigned long int height, unsigned long int width) {
    struct matrix *m = malloc(sizeof(struct matrix));
    if (!m) return NULL;

    m->height = height;
    m->width = width;
    m->rows = aligned_alloc(32, height * width * sizeof(float)); // alinhamento para AVX
    if (!m->rows) {
        free(m);
        return NULL;
    }
    return m;
}

// Libera memória
void free_matrix(struct matrix *m) {
    if (m) {
        free(m->rows);
        free(m);
    }
}

// Lê matriz de arquivo binário
int load_matrix_from_file(struct matrix *m, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    size_t n = m->height * m->width;
    fread(m->rows, sizeof(float), n, f);
    fclose(f);
    return 1;
}

// Salva matriz em arquivo binário
int save_matrix_to_file(struct matrix *m, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    size_t n = m->height * m->width;
    fwrite(m->rows, sizeof(float), n, f);
    fclose(f);
    return 1;
}

// Imprime até 256 elementos
void print_matrix_sample(struct matrix *m) {
    unsigned long int total = m->height * m->width;
    unsigned long int limit = total > 256 ? 256 : total;
    for (unsigned long int i = 0; i < limit; i++) {
        printf("%.2f ", m->rows[i]);
        if ((i + 1) % m->width == 0) printf("\n");
    }
    printf("\n");
}

// Multiplicação escalar * matriz (AVX)
int scalar_matrix_mult(float scalar_value, struct matrix *matrix) {
    if (!matrix || !matrix->rows) return 0;

    unsigned long int size = matrix->height * matrix->width;
    unsigned long int i;
    __m256 scalar_vec = _mm256_set1_ps(scalar_value);

    for (i = 0; i + 8 <= size; i += 8) {
        __m256 vec = _mm256_loadu_ps(&matrix->rows[i]);
        __m256 result = _mm256_mul_ps(vec, scalar_vec);
        _mm256_storeu_ps(&matrix->rows[i], result);
    }

    for (; i < size; i++) {
        matrix->rows[i] *= scalar_value;
    }

    return 1;
}

// Multiplicação de matrizes (A x B = C) com AVX/FMA
int matrix_matrix_mult(struct matrix *matrixA, struct matrix *matrixB, struct matrix *matrixC) {
    if (!matrixA || !matrixB || !matrixC) return 0;
    if (matrixA->width != matrixB->height) return 0;

    unsigned long int M = matrixA->height;
    unsigned long int N = matrixA->width;
    unsigned long int P = matrixB->width;

    for (unsigned long int i = 0; i < M; i++) {
        for (unsigned long int j = 0; j < P; j++) {
            __m256 sum_vec = _mm256_setzero_ps();

            unsigned long int k;
            for (k = 0; k + 8 <= N; k += 8) {
                __m256 a_vec = _mm256_loadu_ps(&matrixA->rows[i * N + k]);
                __m256 b_vec = _mm256_loadu_ps(&matrixB->rows[k * P + j]);
                sum_vec = _mm256_fmadd_ps(a_vec, b_vec, sum_vec);
            }

            float temp[8];
            _mm256_storeu_ps(temp, sum_vec);

            float sum = 0.0f;
            for (int t = 0; t < 8; t++) sum += temp[t];

            for (; k < N; k++) {
                sum += matrixA->rows[i * N + k] * matrixB->rows[k * P + j];
            }

            matrixC->rows[i * P + j] = sum;
        }
    }

    return 1;
}
