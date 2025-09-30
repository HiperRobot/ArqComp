#include <stdio.h>
#include <stdlib.h>

int main() {
    float *arr = malloc(1024*1024*sizeof(float));
    for (int i = 0; i < 1024*1024; i++){
        *(arr+i) = (float) i*1 + 1.0;
    }
    size_t n = 1024*1024;

    // Abre o arquivo em modo binário de escrita
    FILE *f = fopen("floats_256_5.0f.dat", "wb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    // Escreve o array inteiro no arquivo
    size_t written = fwrite(arr, sizeof(float), n, f);
    if (written != n) {
        perror("Erro ao escrever no arquivo");
        fclose(f);
        return 1;
    }

    fclose(f);
    printf("Arquivo gravado com sucesso.\n");

    return 0;
}