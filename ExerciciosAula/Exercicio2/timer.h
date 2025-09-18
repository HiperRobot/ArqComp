#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

// função que retorna tempo atual em segundos
double wtime(void);

// função que retorna a diferença entre dois timeval em milissegundos
double timedifference_msec(struct timeval t0, struct timeval t1);

#endif
