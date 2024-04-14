#include <stdio.h>
#include <stdlib.h>
#include "sampler.h"
#include "time.h"
#include "util.h"

int main(int argc, char **argv) {
    int sampler = 0;
    double sigma = 1.0, mu = 0;
    uint64_t samples = 0;
    parse_arg(argc, argv, &sampler, &sigma, &mu, &samples);

    sampler_shake256_context rng;
    sampler_context sc;
    char seeds[4][16] = {"sampler_1", "sampler_2", "sampler_3", "sampler_4"};
    char file_name[32] = {0};

    printf("Write sampler to file\n");
    fflush(stdout);

    clock_t start, end;
    double duration;
    start = clock();
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)seeds[sampler], strlen(seeds[sampler]));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    sprintf(file_name, "sampler_%d_%lu.txt", sampler, samples);
    FILE *f = fopen(file_name, "w");
    fprintf(f, "mu = 0, sigma = 0.75\n");
    switch (sampler)
    {
    case 1:
        for(uint64_t i = 0; i < samples; ++i)
            fprintf(f, "%d, ", sampler_1(&sc));
        break;
    case 2:
        for(uint64_t i = 0; i < samples; ++i)
            fprintf(f, "%d, ", sampler_2(&sc));
        break;
    case 3:
        for(uint64_t i = 0; i < samples; ++i)
            fprintf(f, "%d, ", sampler_3(&sc, mu));
        break;
    case 4:
        for(uint64_t i = 0; i < samples; ++i)
            fprintf(f, "%d, ", sampler_4(&sc, sigma, mu));
        break;
    default:
        break;
    }
    fclose(f);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %fs\n", duration);

    return 0;
}