#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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

    float *a = (float*) malloc(n * sizeof(float));
    float *b = (float*) malloc(n * sizeof(float));
    float *c = (float*) malloc(n * sizeof(float));
    float *r = (float*) malloc(n * sizeof(float));

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

    // Operação r = a*b + c
    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i++) {
        r[i] = a[i] * b[i] + c[i];
    }
    gettimeofday(&stop, NULL);
    printf("Tempo da multiplicação + soma (sem AVX): %f ms\n", timedifference_msec(start, stop));

    // Validação
    int erros = 0;
    for (int i = 0; i < n; i++) {
        if (r[i] != 13.0f) { // 5*2 + 3 = 13
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
// rodar com:
// gcc -std=c11 -O2 -o muladd_sem_avx sem_avx_muladd.c timer.c
//./muladd_sem_avx 1000000