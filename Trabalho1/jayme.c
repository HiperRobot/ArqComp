#include <stdlib.h>
#include "matrix_lib.h"

int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
    if (matrix == NULL)
        return 0;
    unsigned long int h = matrix->height;
    unsigned long int w = matrix->width;
    if (h <= 0 || w <= 0)
        return 0;
    
    for (unsigned long int i = 0; i<h*w; i++)
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
    
    unsigned long int dim[6] = {hA, wA, hB, wB, hC, wC};
    for (unsigned long int i = 0; i < 6; i++){
        if (dim[i] <= 0)
            return 0;
    }

    if (wA != hB || hC != hA || wC != wB)
        return 0;

    for (unsigned long int k = 0; k < hA; k++) {        // linha de A e C
        for (unsigned long int j = 0; j < wB; j++) {    // coluna de B e C
            for (unsigned long int i = 0; i < wA; i++) { // percorre elementos da linha/coluna
                matrixC->rows[k*wC + j] += matrixA->rows[k*wA + i] * matrixB->rows[i*wB + j];
            }
        }
    }

    return 1;
};
