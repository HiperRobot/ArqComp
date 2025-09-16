#include <stdio.h>
#include <stdlib.h>

void create_test_file(const char* filename, unsigned long int size, float value) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error creating file %s\n", filename);
        return;
    }

    for (unsigned long int i = 0; i < size; i++) {
        fwrite(&value, sizeof(float), 1, file);
    }
    fclose(file);
}

int main() {
    // Create test files for 8x16 and 16x8 matrices
    create_test_file("floats_256_2.0f.dat", 8 * 16, 2.0f);
    create_test_file("floats_256_5.0f.dat", 16 * 8, 5.0f);
    return 0;
}