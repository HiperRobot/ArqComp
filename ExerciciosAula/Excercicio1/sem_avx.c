#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "timer.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;

    /* Verifica argumentos */
    if (argc != 2) {
        printf("Uso: %s <tamanho do array>\n", argv[0]);
        return 1;
    }

    /* Converte argumentos */
    int n = atoi(argv[1]);
    if (n % 8 != 0) {
        printf("Erro: o tamanho do array deve ser múltiplo de 8.\n");
        return 1;
    }

    /* Aloca os três arrays em memória */
    float *par = (float*) malloc(n * sizeof(float));
    float *impar = (float*) malloc(n * sizeof(float));
    float *resultado = (float*) malloc(n * sizeof(float));

    /* Inicializa os dois arrays em memória */
    printf("Inicializando arrays...\n");
    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i++) {
        par[i] = 8.0f + i;
        impar[i] = 5.0f;
    }
    gettimeofday(&stop, NULL);
    printf("Tempo de inicialização: %f ms\n", timedifference_msec(start, stop));

    /* Executa a subtração dos elementos dos arrays: resultado = par - impar */
    printf("Executando subtração...\n");
    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i++) {
        resultado[i] = par[i] - impar[i];
    }
    gettimeofday(&stop, NULL);
    printf("Tempo da subtração: %f ms\n", timedifference_msec(start, stop));

    /* Verifica se ocorreu algum erro na subtração de algum elemento */
    int ok = 1;
    for (int i = 0; i < n; i++) {
        float esperado = par[i] - impar[i];
        if (resultado[i] != esperado) {
            printf("Erro em resultado[%d] = %f (esperado %f)\n", i, resultado[i], esperado);
            ok = 0;
            break;
        }
    }

    if (ok) {
        printf("Validação concluída com sucesso!\n");
        printf("Primeiros valores do resultado:\n");
        for (int i = 0; i < (n < 16 ? n : 16); i++) {
            printf("resultado[%d] = %.2f\n", i, resultado[i]);
        }
    }

    /* Libera a memória alocada pelos arrays em memória */
    free(par);
    free(impar);
    free(resultado);

    return 0;
}
