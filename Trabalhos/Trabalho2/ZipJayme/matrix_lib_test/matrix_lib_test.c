#include <stdlib.h>
#include <stdio.h>
#include "matrix_lib.h"
#include "timer.h"

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

    printf("\n\nPrimeiros 256 elementos da matrixA:\n\n");
    printf("[");
    for (int i = 0; i < (256 < widthA*heightA ? 256 : widthA*heightA); i++){
        printf("%f", *(arrMatrixA+i));
        if (i == (256 < widthA*heightA ? 256 : widthA*heightA)-1){
            printf("]");
        }else{
            printf(", ");
        }
    }

    printf("\n\nPrimeiros 256 elementos da matrixB:\n\n");
    printf("[");
    for (int i = 0; i < (256 < widthB*heightB ? 256 : widthB*heightB); i++){
        printf("%f", *(arrMatrixB+i));
        if (i == (256 < widthB*heightB ? 256 : widthB*heightB)-1){
            printf("]");
        }else{
            printf(", ");
        }
    }
    
    gettimeofday(&start, NULL);
    scalar_matrix_mult(scalar_value, &matrixA);
    gettimeofday(&stop, NULL);
    printf("\n\nScalar mult time: %f ms\n", timedifference_msec(start, stop));

    printf("\n\nPrimeiros 256 elementos da matrixA (após multiplicação por scalar):\n\n");
    printf("[");
    for (int i = 0; i < (256 < widthA*heightA ? 256 : widthA*heightA); i++){
        printf("%f", *(arrMatrixA+i));
        if (i == (256 < widthA*heightA ? 256 : widthA*heightA)-1){
            printf("]");
        }else{
            printf(", ");
        }
    }

    gettimeofday(&start, NULL);
    matrix_matrix_mult(&matrixA, &matrixB, &matrixC);
    gettimeofday(&stop, NULL);
    printf("\n\nMatrix mult time: %f ms\n", timedifference_msec(start, stop));

    printf("\n\nPrimeiros 256 elementos da matrixC:\n\n");
    printf("[");
    for (int i = 0; i < (256 < widthC*heightC ? 256 : widthC*heightC); i++){
        printf("%f", *(arrMatrixC+i));
        if (i == (256 < widthC*heightC ? 256 : widthC*heightC)-1){
            printf("]");
        }else{
            printf(", ");
        }
    }

    FILE *file = fopen(resultScalar, "wb");
    fwrite(matrixA.rows, sizeof(float), widthA*heightA, file); // escreve todos os floats
    fclose(file);

    file = fopen(resultMultMatrix, "wb");
    fwrite(matrixC.rows, sizeof(float), widthC*heightC, file); // escreve todos os floats
    fclose(file);

    gettimeofday(&overall_t2, NULL);
    printf("\n\nOverall time: %f ms\n", timedifference_msec(overall_t1, overall_t2));
    printf("\nProcessador usado nos testes: 12th Gen Intel(R) Core(TM) i7-12700H");
}
