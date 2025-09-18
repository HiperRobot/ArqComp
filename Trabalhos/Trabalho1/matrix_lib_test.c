#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "matrix_lib.h"
#include "timer.h"

void print_matrix_portion(struct matrix *m, const char *name) {
    printf("\nFirst elements of %s:\n", name);
    int elements_to_print = (m->height * m->width < 256) ? m->height * m->width : 256;
    for (int i = 0; i < elements_to_print; i++) {
        printf("%.2f ", m->rows[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    struct timeval start, stop, overall_t1, overall_t2;
    gettimeofday(&overall_t1, NULL);

    float scalar_value = strtof(argv[1], NULL);
    unsigned long int heightA = strtoul(argv[2], NULL, 10);
    unsigned long int widthA = strtoul(argv[3], NULL, 10);
    unsigned long int heightB = strtoul(argv[4], NULL, 10);
    unsigned long int widthB = strtoul(argv[5], NULL, 10);
    char* arqMatrixA = argv[6];
    char* arqMatrixB = argv[7];
    char* resultScalar = argv[8];
    char* resultMultMatrix = argv[9];
    FILE *fileMatrixA = fopen("floats_256_2.0f.dat", "rb");
    FILE *fileMatrixB = fopen("floats_256_5.0f.dat", "rb");

    fseek(fileMatrixA, 0, SEEK_END);
    long size = ftell(fileMatrixA);
    fseek(fileMatrixA, 0, SEEK_SET);
    size_t n = size / sizeof(float);
    float *arrMatrixA = malloc(n * sizeof(float));
    size_t read = fread(arrMatrixA, sizeof(float), n, fileMatrixA);
    fclose(fileMatrixA);

    fseek(fileMatrixB, 0, SEEK_END);
    size = ftell(fileMatrixB);
    fseek(fileMatrixB, 0, SEEK_SET);
    n = size / sizeof(float);
    float *arrMatrixB = malloc(n * sizeof(float));
    read = fread(arrMatrixB, sizeof(float), n, fileMatrixB);
    fclose(fileMatrixB);

    unsigned long int heightC = heightA;
    unsigned long int widthC = widthB;
    float *arrMatrixC = malloc((heightC*widthC) * sizeof(float));

    for (int i = 0; i<widthC*heightC; i++)
        *(arrMatrixC+i) = 0.0;

    struct matrix matrixA = {heightA, widthA, arrMatrixA};
    struct matrix matrixB = {heightB, widthB, arrMatrixB};
    struct matrix matrixC = {heightC, widthC, arrMatrixC};

    gettimeofday(&start, NULL);
    scalar_matrix_mult(scalar_value, &matrixA);
    gettimeofday(&stop, NULL);
    printf("Scalar mult time: %f ms\n", timedifference_msec(start, stop));

    gettimeofday(&start, NULL);
    matrix_matrix_mult(&matrixA, &matrixB, &matrixC);
    gettimeofday(&stop, NULL);
    printf("Matrix mult time: %f ms\n", timedifference_msec(start, stop));

    FILE *file = fopen(resultScalar, "wb");
    fwrite(matrixA.rows, sizeof(float), widthA*heightA, file); // escreve todos os floats
    fclose(file);

    file = fopen(resultMultMatrix, "wb");
    fwrite(matrixC.rows, sizeof(float), widthC*heightC, file); // escreve todos os floats
    fclose(file);
}
