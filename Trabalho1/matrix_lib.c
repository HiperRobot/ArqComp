#include <stdio.h>
#include <stdlib.h>
#include "matrix_lib.h"



int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
    if (matrix == NULL)
        return 0;
    unsigned long int h = matrix->height;
    unsigned long int w = matrix->width;
    if (h <= 0 || w <= 0)
        return 0;

    // Check if dimensions are multiples of 8
    if (h % 8 != 0 || w % 8 != 0)
        printf("Scalar: Dimensions not multiple of 8\n");
        return 0;

    for (int i = 0; i<h*w; i++)
        matrix->rows[i] = matrix->rows[i]*scalar_value;

    return 1;
};

int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC){
    if (matrixA == NULL)
        return 0;
    unsigned long int hA = matrixA->height;
    unsigned long int wA = matrixA->width;

    if (matrixB == NULL)
        return 0;
    unsigned long int hB = matrixB->height;
    unsigned long int wB = matrixB->width;

    if (matrixC == NULL)
        return 0;
    unsigned long int hC = matrixC->height;
    unsigned long int wC = matrixC->width;

    // Check if dimensions are valid
    if (hA <= 0 || wA <= 0 || hB <= 0 || wB <= 0 || hC <= 0 || wC <= 0)
        printf("Invalid matrix dimensions\n");
        return 0;

    // Check if dimensions are multiples of 8
    if (hA % 8 != 0 || wA % 8 != 0 || hB % 8 != 0 || wB % 8 != 0 || hC % 8 != 0 || wC % 8 != 0)
        printf("Matrix: Dimensions not multiple of 8\n");
        return 0;

    // Check matrix multiplication compatibility
    if (wA != hB || hC != hA || wC != wB)
        printf("Incompatible matrix dimensions\n");
        return 0;

    // Initialize result matrix to zero
    for (unsigned long int i = 0; i < hC * wC; i++) {
        matrixC->rows[i] = 0.0f;
    }

    printf("Starting matrix multiplication of %lux%lu and %lux%lu matrices...\n", hA, wA, hB, wB);

    // Perform matrix multiplication
    for (unsigned long int i = 0; i < hA; i++) {
        for (unsigned long int j = 0; j < wB; j++) {
            float sum = 0.0f;
            for (unsigned long int k = 0; k < wA; k++) {
                sum += matrixA->rows[i * wA + k] * matrixB->rows[k * wB + j];
            }
            matrixC->rows[i * wC + j] = sum;
        }
    }

    return 1;
}