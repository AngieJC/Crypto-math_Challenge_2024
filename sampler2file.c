#include <stdio.h>
#include <stdlib.h>
#include "sampler.h"
#include "time.h"

int main() {
    sampler_shake256_context rng;
    sampler_context sc;
    char *seed1 = "sampler_1";
    char *seed2 = "sampler_2";
    char *seed3 = "sampler_3";
    char *seed4 = "sampler_4";

    printf("Write sampler to file\n");
    fflush(stdout);

    clock_t start, end;
    double duration;
    start = clock();
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)seed1, strlen(seed1));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    int z;
    uint64_t samples = 10000;
    FILE *f = fopen("sampler_1.txt", "w");
    fprintf(f, "mu = 0, sigma = 0.75\n");
    for(uint64_t i = 0; i < samples; ++i)
        fprintf(f, "%d, ", sampler_1(&sc));
    fclose(f);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %fs\n", duration);
    printf("%.0f samples per seccond\n", samples / duration);

    return 0;
}