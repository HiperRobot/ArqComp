#include <stdio.h>

int main() {
    // Test data
    float matrixA[] = {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0
    };

    float matrixB[] = {
        1.0, 2.0,
        3.0, 4.0,
        5.0, 6.0
    };

    // Write matrixA to file
    FILE *f = fopen("matrixA.dat", "wb");
    if (!f) {
        printf("Error creating matrixA.dat\n");
        return 1;
    }
    fwrite(matrixA, sizeof(float), 6, f);
    fclose(f);

    // Write matrixB to file
    f = fopen("matrixB.dat", "wb");
    if (!f) {
        printf("Error creating matrixB.dat\n");
        return 1;
    }
    fwrite(matrixB, sizeof(float), 6, f);
    fclose(f);

    printf("Test files created successfully!\n");
    return 0;
}