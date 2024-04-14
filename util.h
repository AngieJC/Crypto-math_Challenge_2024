#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void usage();

void parse_arg(int argc, char **argv, int *sampler, double *sigma, double *mu, uint64_t *samples);