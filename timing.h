#ifndef TIMING_H
#define TIMING_H

#include <sys/time.h>

#define START_TIMING(a) \
	struct timeval a; \
	gettimeofday(&a, 0);

#define PRINT_TIMING(format, start, ...) \
{ \
	struct timeval __now__; \
	gettimeofday(&__now__, 0); \
	double __elapsedMs__ = (__now__.tv_sec - start.tv_sec) * 1000.0 + ((double) __now__.tv_usec - start.tv_usec) / 1000.0; \
	printf(format, __elapsedMs__, ##__VA_ARGS__); \
}

#endif // TIMING_H
