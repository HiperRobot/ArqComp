#include <sys/time.h>
#include <stddef.h>
#include "timer.h"

double get_time(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}