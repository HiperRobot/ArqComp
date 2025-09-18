#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix_lib.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    if (argc != 9) {
        printf("Uso: %s <escalar> <A_height> <A_width> <B_height> <B_width> <arquivoA> <arquivoB> <arquivoOut1> <arquivoOut2>\n", argv[0]);
        return 1;
    }

    float scalar = atof(argv[1]);
    unsigned long int Ah = atol(argv[2]);
    unsigned long int Aw = atol(argv[3]);
    unsigned long int Bh = atol(argv[4]);
    unsigned long int Bw = atol(argv[5]);

    char *fileA = argv[6];
    char *fileB = argv[7];
    char *fileOut1 = argv[8];
    char *fileOut2 = argv[9];

    struct timeval start, stop;
    double elapsed;

    // Aloca matrizes
    struct matrix *A = allocate_matrix(Ah, Aw);
    struct matrix *B = allocate_matrix(Bh, Bw);
    struct matrix *C = allocate_matrix(Ah, Bw);

    if (!A || !B || !C) {
        printf("Erro ao alocar matrizes\n");
        return 1;
    }

    // Carrega arquivos
    load_matrix_from_file(A, fileA);
    load_matrix_from_file(B, fileB);

    printf("Modelo do processador:\n");
    system("lscpu | grep 'Model name'");

    gettimeofday(&start, NULL);
    scalar_matrix_mult(scalar, A);
    save_matrix_to_file(A, fileOut1);
    gettimeofday(&stop, NULL);
    elapsed = timedifference_msec(start, stop);
    printf("Tempo scalar_matrix_mult: %f ms\n", elapsed);

    gettimeofday(&start, NULL);
    matrix_matrix_mult(A, B, C);
    save_matrix_to_file(C, fileOut2);
    gettimeofday(&stop, NULL);
    elapsed = timedifference_msec(start, stop);
    printf("Tempo matrix_matrix_mult: %f ms\n", elapsed);

    printf("Amostra da matriz A:\n");
    print_matrix_sample(A);

    printf("Amostra da matriz B:\n");
    print_matrix_sample(B);

    printf("Amostra da matriz C:\n");
    print_matrix_sample(C);

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}
