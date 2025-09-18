#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc < 10) {
        printf("Uso: %s scalar heightA widthA heightB widthB arqA arqB resultScalar resultMultMatrix\n", argv[0]);
        return 1;
    }

    float scalar_value = strtof(argv[1], NULL);
    unsigned long int heightA = strtoul(argv[2], NULL, 10);
    unsigned long int widthA  = strtoul(argv[3], NULL, 10);
    unsigned long int heightB = strtoul(argv[4], NULL, 10);
    unsigned long int widthB  = strtoul(argv[5], NULL, 10);
    char* arqMatrixA = argv[6];
    char* arqMatrixB = argv[7];
    char* resultScalar = argv[8];
    char* resultMultMatrix = argv[9];

    // Buffers separados
    float *bufferA = malloc(heightA * widthA * sizeof(float));
    float *bufferB = malloc(heightB * widthB * sizeof(float));
    float *bufferResultScalar = malloc(heightA * widthA * sizeof(float));
    float *bufferResultMult = malloc(heightA * widthB * sizeof(float));

    // Lendo MatrixA
    FILE *f = fopen(arqMatrixA, "rb");
    fread(bufferA, sizeof(float), heightA*widthA, f);
    fclose(f);

    printf("\nMatrixA:\n");
    for (unsigned long int i = 0; i < heightA; i++) {
        for (unsigned long int j = 0; j < widthA; j++) {
            printf("%.2f ", bufferA[i*widthA + j]);
        }
        printf("\n");
    }

    // Lendo MatrixB
    f = fopen(arqMatrixB, "rb");
    fread(bufferB, sizeof(float), heightB*widthB, f);
    fclose(f);

    printf("\nMatrixB:\n");
    for (unsigned long int i = 0; i < heightB; i++) {
        for (unsigned long int j = 0; j < widthB; j++) {
            printf("%.2f ", bufferB[i*widthB + j]);
        }
        printf("\n");
    }

    // Lendo resultado escalar
    f = fopen(resultScalar, "rb");
    fread(bufferResultScalar, sizeof(float), heightA*widthA, f);
    fclose(f);

    printf("\nMatrixA * %.2f:\n", scalar_value);
    for (unsigned long int i = 0; i < heightA; i++) {
        for (unsigned long int j = 0; j < widthA; j++) {
            printf("%.2f ", bufferResultScalar[i*widthA + j]);
        }
        printf("\n");
    }

    // Lendo resultado multiplicação de matrizes
    f = fopen(resultMultMatrix, "rb");
    fread(bufferResultMult, sizeof(float), heightA*widthB, f);
    fclose(f);

    printf("\nMatrixC (MatrixA x MatrixB):\n");
    for (unsigned long int i = 0; i < heightA; i++) {
        for (unsigned long int j = 0; j < widthB; j++) {
            printf("%.2f ", bufferResultMult[i*widthB + j]);
        }
        printf("\n");
    }

    // Liberando memória
    free(bufferA);
    free(bufferB);
    free(bufferResultScalar);
    free(bufferResultMult);

    return 0;
}