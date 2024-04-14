#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>

#define VMRSS_LINE 22//VMRSS所在行, 注:根据不同的系统,位置可能有所区别.
#define pid_t int

void usage();

void parse_arg(int argc, char **argv, int *sampler, double *sigma, double *mu, uint64_t *samples);

int get_phy_mem(const pid_t p);
int get_rmem(pid_t p);
int get_total_mem();