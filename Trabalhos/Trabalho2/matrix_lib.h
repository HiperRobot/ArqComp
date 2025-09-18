#ifndef MATRIX_LIB_H
#define MATRIX_LIB_H

struct matrix {
    unsigned long int height;
    unsigned long int width;
    float *rows;
};

// Funções pedidas no enunciado
int scalar_matrix_mult(float scalar_value, struct matrix *matrix);
int matrix_matrix_mult(struct matrix *matrixA, struct matrix *matrixB, struct matrix *matrixC);

// Funções auxiliares
struct matrix *allocate_matrix(unsigned long int height, unsigned long int width);
void free_matrix(struct matrix *m);
int load_matrix_from_file(struct matrix *m, const char *filename);
int save_matrix_to_file(struct matrix *m, const char *filename);
void print_matrix_sample(struct matrix *m);

#endif
