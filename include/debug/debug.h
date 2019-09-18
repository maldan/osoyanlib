#pragma once

#include <time.h>
#include <sys/time.h>

#define START_TIME(X) struct timeval X##1, X##2; gettimeofday(&X##1, NULL)
#define RESET_TIME(X) gettimeofday(&X##1, NULL)
#define END_TIME(X) gettimeofday(&X##2, NULL); \
printf("Total time = %f seconds\n", (double) ((X##2).tv_usec - (X##1).tv_usec) / 1000000 + (double) ((X##2).tv_sec - (X##1).tv_sec))
