#include <stdlib.h>
#include <stdio.h>
#include "matrix_lib.h"

int main(int argc, char *argv[]){
    float scalar_value = strtof(argv[1], NULL);
    unsigned long int heightA = strtoul(argv[2], NULL, 10);
    unsigned long int widthA = strtoul(argv[3], NULL, 10);
    unsigned long int heightB = strtoul(argv[4], NULL, 10);
    unsigned long int widthB = strtoul(argv[5], NULL, 10);
    char* arqMatrixA = argv[6];
    char* arqMatrixB = argv[7];
    char* resultScalar = argv[8];
    char* resultMultMatrix = argv[9];
}