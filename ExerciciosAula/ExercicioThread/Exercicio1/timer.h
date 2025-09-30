#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>

// Define the GET_TIME macro
#define GET_TIME(now) { \
    struct timeval t; \
    gettimeofday(&t, NULL); \
    now = t.tv_sec + t.tv_usec/1000000.0; \
}

double get_time(void);
double timedifference_msec(struct timeval t0, struct timeval t1);

#endif