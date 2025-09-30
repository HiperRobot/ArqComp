struct matrix;

int scalar_matrix_mult(float scalar_value, struct matrix *matrix);
int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC);

typedef struct matrix {
    int height;
    int width;
    float *rows;
} matrix;
