#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *evens, *odds, *result;
int num_threads, array_size;

typedef struct {
    int start;
    int end;
} thread_data;

/* Thread para inicializar arrays */
void *init_arrays(void *arg) {
    thread_data *data = (thread_data *) arg;
    for (int i = data->start; i < data->end; i++) {
        evens[i] = 2.0f;
        odds[i]  = 5.0f;
    }
    pthread_exit(NULL);
}

/* Thread para multiplicar arrays */
void *mult_arrays(void *arg) {
    thread_data *data = (thread_data *) arg;
    for (int i = data->start; i < data->end; i++) {
        result[i] = evens[i] * odds[i];
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <num_threads> <tamanho_array>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);
    array_size = atoi(argv[2]);

    if (array_size % (num_threads * 8) != 0) {
        fprintf(stderr, "Erro: tamanho_array deve ser múltiplo de num_threads*8\n");
        return 1;
    }

    evens  = (float *) malloc(array_size * sizeof(float));
    odds   = (float *) malloc(array_size * sizeof(float));
    result = (float *) malloc(array_size * sizeof(float));

    pthread_t threads[num_threads];
    thread_data tdata[num_threads];
    double start, finish;

    /* Inicialização */
    GET_TIME(start);
    for (int t = 0; t < num_threads; t++) {
        tdata[t].start = t * (array_size / num_threads);
        tdata[t].end   = (t+1) * (array_size / num_threads);
        pthread_create(&threads[t], NULL, init_arrays, &tdata[t]);
    }
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
    GET_TIME(finish);
    printf("Tempo para inicialização: %f segundos\n", finish - start);

    /* Multiplicação */
    GET_TIME(start);
    for (int t = 0; t < num_threads; t++) {
        pthread_create(&threads[t], NULL, mult_arrays, &tdata[t]);
    }
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
    GET_TIME(finish);
    printf("Tempo para multiplicação: %f segundos\n", finish - start);

    /* Validação sequencial */
    for (int i = 0; i < array_size; i++) {
        if (result[i] != 10.0f) {
            printf("Erro na posição %d: %f\n", i, result[i]);
            break;
        }
    }

    free(evens);
    free(odds);
    free(result);
    return 0;
}
