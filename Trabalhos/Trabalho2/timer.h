#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

double wtime(void);
double timedifference_msec(struct timeval t0, struct timeval t1);

#endif // TIMER_H
