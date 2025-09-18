// matrix_lib.c
#include "matrix_lib.h"
#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>

/*
Implementation notes:
- N must be multiple of 8
- Uses aligned loads/stores and AVX/FMA intrinsics
- matrix layout: row-major, element A[i*N + j]
*/

/* scalar * matrix: R = scalar * A */
void scalar_matrix_mult(float scalar, const float *A, float *R, int N) {
    int total = N * N;
    int i = 0;
    __m256 vscalar = _mm256_set1_ps(scalar);

    /* process 8 floats at a time */
    for (i = 0; i < total; i += 8) {
        __m256 va = _mm256_load_ps(&A[i]);
        __m256 vr = _mm256_mul_ps(vscalar, va);
        _mm256_store_ps(&R[i], vr);
    }
}

/* matrix multiplication C = A * B naive blocked ijk with vectorized inner loop that processes 8 columns at a time.
   Block size chosen to be multiple of 8 and friendly with caches. */
void matrix_matrix_mult(const float *A, const float *B, float *C, int N) {
    // initialize C to zero
    int total = N * N;
    for (int i = 0; i < total; i += 8) {
        _mm256_store_ps(&C[i], _mm256_setzero_ps());
    }

    const int BS = 64; // blocking size (must be multiple of 8). Tune if needed.

    for (int ii = 0; ii < N; ii += BS) {
        int iimax = (ii + BS < N) ? ii + BS : N;
        for (int kk = 0; kk < N; kk += BS) {
            int kkmax = (kk + BS < N) ? kk + BS : N;
            for (int jj = 0; jj < N; jj += BS) {
                int jjmax = (jj + BS < N) ? jj + BS : N;

                // Block (ii..iimax-1) x (jj..jjmax-1) with k over (kk..kkmax-1)
                for (int i = ii; i < iimax; ++i) {
                    for (int k = kk; k < kkmax; ++k) {
                        // scalar a_ik
                        float a_scalar = A[i * N + k];
                        __m256 va = _mm256_set1_ps(a_scalar); // broadcast

                        // process 8 columns of B/C at once
                        int j = jj;
                        for (; j + 7 < jjmax; j += 8) {
                            // load B[k, j..j+7]
                            __m256 vb = _mm256_load_ps(&B[k * N + j]);
                            // load C[i, j..j+7]
                            __m256 vc = _mm256_load_ps(&C[i * N + j]);
                            // vc += va * vb  (fused multiply-add)
                            vc = _mm256_fmadd_ps(va, vb, vc);
                            _mm256_store_ps(&C[i * N + j], vc);
                        }
                        // no tail expected because N multiple of 8; if not, handle here
                    }
                }

            }
        }
    }
}
