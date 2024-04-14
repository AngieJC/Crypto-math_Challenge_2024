#include "util.h"

void usage() {
    printf("Usage:\n");
    printf("./elf (integer[1,4])<sampler> [sigma] [mu] (int[1,u64_max])<num of sample>\n");
    exit(EXIT_FAILURE);
}

void parse_arg(int argc, char **argv, int *sampler, double *sigma, double *mu, uint64_t *samples) {
    if(argc < 3)
        usage();
    *sampler = atoi(argv[1]);
    if(*sampler < 1 || *sampler > 4)
        usage();
    if(*sampler <= 2)
        *samples = strtoull(argv[2], NULL, 10);
    else if(*sampler == 3) {
        *mu = strtoull(argv[2], NULL, 10);
        *samples = strtoull(argv[3], NULL, 10);
    }
    else {
        *sigma = strtod(argv[2], NULL);
        *mu = strtod(argv[3], NULL);
        *samples = strtoull(argv[4], NULL, 10);
    }
    if(*sigma <= 0.8 || *sigma >= 1.6 || *mu < 0 || *mu >= 1) {
        printf("sigma or mu value err\n");
        exit(EXIT_FAILURE);
    }
}