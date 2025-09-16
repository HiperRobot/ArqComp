#include <stdio.h>
#include <stdlib.h>

int main () {
    int n = 1000000; // Number of elements in the arrays
    float *a = (float*) aligned_alloc(32, n * sizeof(float));
    float *b = (float*) aligned_alloc(32, n * sizeof(float));
    float *c = (float*) aligned_alloc(32, n * sizeof(float));

    // Initialize arrays a and b
    for (int i = 0; i < n; i++) {
        a[i] = (float)i;
        b[i] = (float)(n - i);
    }

    // Perform subtraction using AVX intrinsics
    #pragma omp parallel for
    for (int i = 0; i < n; i += 8) {
        __m256 va = _mm256_load_ps(&a[i]);
        __m256 vb = _mm256_load_ps(&b[i]);
        __m256 vc = _mm256_sub_ps(va, vb);
        _mm256_store_ps(&c[i], vc);
    }

    // Print first 10 results
    for (int i = 0; i < 10; i++) {
        printf("c[%d] = %f\n", i, c[i]);
    }

    free(a);
    free(b);
    free(c);
    return 0;
}

