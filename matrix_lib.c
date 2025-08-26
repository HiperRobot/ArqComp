#include <stdlib.h>
#include "matrix_lib.h"



int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
    if (matrix == NULL)
        return 0;
    unsigned long int h = matrix->height;
    unsigned long int w = matrix->width;
    if (h <= 0 || w <= 0)
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

    unsigned long int dim[] = {hA, wA, hB, wB, hC, wC};
    for (int i = 0; i < 6; i++){
        if (dim[i] <= 0)
            return 0;
    }

    if (wA != hB || hC != hA || wC != wB)
        return 0;

    for(int i = 0; i < wA; i++){
        for (int j = 0; j < wB; j++){
            for (int k = 0; k < hA; k++){
                matrixC->rows[k*wC+j] += (matrixA->rows[k*wA+i]) * (matrixB->rows[i*wB+j]);
            }
        }
    }

    return 1;
};