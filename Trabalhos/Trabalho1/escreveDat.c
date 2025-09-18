#include <stdio.h>

int main() {
    float arr[] = {-2.0f, 1.0f, 0.0f, 2.0f, 2.0f, -1.0f};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    // Abre o arquivo em modo binário de escrita
    FILE *f = fopen("floats_256_2.0f.dat", "wb");
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