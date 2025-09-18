// test_matrix.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "timer.h"
#include "matrix_lib.h"

#define PRINT_LIMIT 256

static void print_matrix_sample(const char *name, const float *M, int N) {
    int count = (N * N < PRINT_LIMIT) ? (N * N) : PRINT_LIMIT;
    printf("Sample of %s (up to %d elements):\n", name, count);
    for (int idx = 0; idx < count; ++idx) {
        printf("%s[%d] = %.2f\n", name, idx, M[idx]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <N>  (N deve ser multiplo de 8)\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N % 8 != 0) {
        printf("Erro: N deve ser multiplo de 8\n");
        return 1;
    }

    size_t bytes = (size_t)N * (size_t)N * sizeof(float);
    printf("Matrix size: %d x %d  (%.2f MB per matrix)\n", N, N, (double)bytes / (1024.0*1024.0));

    // allocate aligned
    float *A = (float*) aligned_alloc(32, bytes);
    float *B = (float*) aligned_alloc(32, bytes);
    float *C = (float*) aligned_alloc(32, bytes);
    float *R = (float*) aligned_alloc(32, bytes); // for scalar * matrix result

    if (!A || !B || !C || !R) {
        fprintf(stderr, "Erro alocacao memoria\n");
        return 1;
    }

    struct timeval tstart, tstop;
    double overall_start = wtime();

    // print CPU model
    printf("CPU model: ");
    fflush(stdout);
    system("lscpu | grep 'Model name'");

    // initialize A and B and C (C will store A*B)
    printf("Inicializando matrizes...\n");
    gettimeofday(&tstart, NULL);
    for (int i = 0; i < N * N; ++i) {
        A[i] = 1.0f; // you can change initialization pattern if desired
        B[i] = 2.0f;
        C[i] = 0.0f;
        R[i] = 0.0f;
    }
    gettimeofday(&tstop, NULL);
    printf("Tempo de inicializacao: %f ms\n", timedifference_msec(tstart, tstop));

    // scalar * matrix (R = scalar * A)
    float scalar = 3.14f;
    printf("Executando scalar_matrix_mult (R = %.2f * A)...\n", scalar);
    gettimeofday(&tstart, NULL);
    scalar_matrix_mult(scalar, A, R, N);
    gettimeofday(&tstop, NULL);
    printf("Tempo scalar_matrix_mult: %f ms\n", timedifference_msec(tstart, tstop));

    // matrix * matrix (C = A * B)
    printf("Executando matrix_matrix_mult (C = A * B)...\n");
    gettimeofday(&tstart, NULL);
    matrix_matrix_mult(A, B, C, N);
    gettimeofday(&tstop, NULL);
    printf("Tempo matrix_matrix_mult: %f ms\n", timedifference_msec(tstart, tstop));

    double overall_end = wtime();
    printf("Overall time: %.6f seconds\n", overall_end - overall_start);

    // Print samples (up to 256 elements)
    print_matrix_sample("A", A, N);
    print_matrix_sample("B", B, N);
    print_matrix_sample("C", C, N);
    print_matrix_sample("R", R, N);

    // Validate a few entries of C: since A=1 and B=2, each C[i] should equal sum over k of (1*2) = 2*N
    // So each element of C (in this init) is 2*N. Validate first few.
    int errors = 0;
    float expectC = 2.0f * (float)N;
    for (int i = 0; i < (N < 8 ? N : 8); ++i) {
        if (C[i] != expectC) errors++;
    }
    if (errors == 0) printf("Validação C ok (cheque inicial)\n");
    else printf("Validação C falhou (erros: %d)\n", errors);

    free(A); free(B); free(C); free(R);
    return 0;
}
