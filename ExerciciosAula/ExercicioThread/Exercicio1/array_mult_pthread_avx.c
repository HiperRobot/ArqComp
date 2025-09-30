#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <immintrin.h>
#include "timer.h"

float *evens, *odds, *result;
int num_threads, array_size;

typedef struct {
    int start;
    int end;
} thread_data;

/* Inicialização com AVX */
void *init_arrays(void *arg) {
    thread_data *data = (thread_data *) arg;

    // Cria vetores constantes com os valores 2.0f e 5.0f
    __m256 vec_2 = _mm256_set1_ps(2.0f);
    __m256 vec_5 = _mm256_set1_ps(5.0f);

    // Processa 8 elementos por vez usando AVX
    for (int i = data->start; i < data->end; i += 8) {
        _mm256_storeu_ps(&evens[i], vec_2);
        _mm256_storeu_ps(&odds[i], vec_5);
    }

    pthread_exit(NULL);
}

/* Multiplicação com AVX */
void *mult_arrays(void *arg) {
    thread_data *data = (thread_data *) arg;
    for (int i = data->start; i < data->end; i += 8) {
        __m256 v1 = _mm256_loadu_ps(&evens[i]);
        __m256 v2 = _mm256_loadu_ps(&odds[i]);
        __m256 prod = _mm256_mul_ps(v1, v2);
        _mm256_storeu_ps(&result[i], prod);
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

    evens  = (float *) aligned_alloc(32, array_size * sizeof(float));
    odds   = (float *) aligned_alloc(32, array_size * sizeof(float));
    result = (float *) aligned_alloc(32, array_size * sizeof(float));

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
    printf("Tempo para multiplicação (AVX): %f segundos\n", finish - start);

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
