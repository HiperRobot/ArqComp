#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <immintrin.h>
#include "timer.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <tamanho_array>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n % 8 != 0) {
        printf("O tamanho do array deve ser múltiplo de 8.\n");
        return 1;
    }

    float *a = (float*) aligned_alloc(32, n * sizeof(float));
    float *b = (float*) aligned_alloc(32, n * sizeof(float));
    float *c = (float*) aligned_alloc(32, n * sizeof(float));
    float *r = (float*) aligned_alloc(32, n * sizeof(float));

    if (!a || !b || !c || !r) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    struct timeval start, stop;

    // Inicialização
    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i++) {
        a[i] = 5.0f;
        b[i] = 2.0f;
        c[i] = 3.0f;
    }
    gettimeofday(&stop, NULL);
    printf("Tempo de inicialização: %f ms\n", timedifference_msec(start, stop));

    // Operação vetorizada r = a*b + c
    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i += 8) {
        __m256 va = _mm256_load_ps(&a[i]);
        __m256 vb = _mm256_load_ps(&b[i]);
        __m256 vc = _mm256_load_ps(&c[i]);
        __m256 vr = _mm256_fmadd_ps(va, vb, vc); // (a*b + c) usando FMA
        _mm256_store_ps(&r[i], vr);
    }
    gettimeofday(&stop, NULL);
    printf("Tempo da multiplicação + soma (com AVX/FMA): %f ms\n", timedifference_msec(start, stop));

    // Validação
    int erros = 0;
    for (int i = 0; i < n; i++) {
        if (r[i] != 13.0f) {
            erros++;
        }
    }
    if (erros == 0) {
        printf("Validação OK!\n");
    } else {
        printf("Validação falhou: %d erros encontrados.\n", erros);
    }

    free(a);
    free(b);
    free(c);
    free(r);

    return 0;
}
//rodar com:
// gcc -std=c11 -O2 -mavx -mfma -o muladd_com_avx com_avx_muladd.c timer.c
// ./muladd_com_avx 1000000
