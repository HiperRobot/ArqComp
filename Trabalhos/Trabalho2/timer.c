// timer.c
#include "timer.h"
#include <sys/time.h>
#include <stddef.h>

double wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec / 1.0e6;
}

double timedifference_msec(struct timeval t0, struct timeval t1) {
    return (double)(t1.tv_sec - t0.tv_sec) * 1000.0 + (double)(t1.tv_usec - t0.tv_usec) / 1000.0;
}
