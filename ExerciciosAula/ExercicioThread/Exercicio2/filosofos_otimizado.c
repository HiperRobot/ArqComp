#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define NUM_FILOSOFOS	5
#define NUM_RODADAS	5

pthread_mutex_t garfos[NUM_FILOSOFOS];

void pensar(long tid) {
        printf("Filosofo %ld pensando...\n",tid);
        sleep(1);
}

void pegar_garfo(long tid, long gid) {
        printf("Filosofo %ld pegou garfo %ld...\n",tid,gid);
        sleep(1);
}

void largar_garfo(long tid, long gid) {
        printf("Filosofo %ld largou garfo %ld...\n",tid,gid);
        sleep(1);
}

void comer(long tid) {
        printf("Filosofo %ld comendo...\n",tid);
        sleep(1);
}

void *Filosofo(void *t)
{
    int i;
    long tid;
    tid = (long)t;
    for (i=0; i<NUM_RODADAS; ++i) {
        pensar(tid);
        // Previne deadlock fazendo filósofos pares e ímpares pegarem garfos em ordem diferente
        if (tid % 2 == 0) {
            pthread_mutex_lock(&garfos[tid]);
            pthread_mutex_lock(&garfos[(tid+1)%NUM_FILOSOFOS]);
        } else {
            pthread_mutex_lock(&garfos[(tid+1)%NUM_FILOSOFOS]);
            pthread_mutex_lock(&garfos[tid]);
        }
        pegar_garfo(tid,tid);
        pegar_garfo(tid,(tid+1)%NUM_FILOSOFOS);
        comer(tid);
        largar_garfo(tid,tid);
        largar_garfo(tid,(tid+1)%NUM_FILOSOFOS);
        //up/unlock mutexes
        pthread_mutex_unlock(&garfos[tid]);
        pthread_mutex_unlock(&garfos[(tid+1)%NUM_FILOSOFOS]);
    }
    pthread_exit((void*) t);
}

int main (int argc, char *argv[])
{
    pthread_t thread[NUM_FILOSOFOS];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    /* Initialize mutexes for forks */
    for(t=0; t<NUM_FILOSOFOS; t++) {
        pthread_mutex_init(&garfos[t], NULL);
    }

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(t=0; t<NUM_FILOSOFOS; t++) {
        printf("Main: criando o Filosofo %ld\n", t);
        rc = pthread_create(&thread[t], &attr, Filosofo, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_FILOSOFOS; t++) {
        rc = pthread_join(thread[t], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    printf("Main: completou o join do Filosofo %ld com status de %ld\n",t,(long)status);
    }

/* Destroy mutexes before exit */
    for(t=0; t<NUM_FILOSOFOS; t++) {
        pthread_mutex_destroy(&garfos[t]);
    }

printf("Main: Programa completado. Saindo.\n");
pthread_exit(NULL);
}
