/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-04-13 11:22:48
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-04-13 11:39:57
 */
#include <stdio.h>
#include "sampler.h"
#include "time.h"

int main() {
    sampler_shake256_context rng;
    sampler_context sc;
    char *seed1 = "sampler_1";
    char *seed2 = "sampler_2";
    char *seed3 = "sampler_3";
    char *seed4 = "sampler_4";

    printf("Test sampler: \n");
    fflush(stdout);

    clock_t start, end;
    double duration;
    start = clock();
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)seed1, strlen(seed1));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    int z;
    uint64_t samples = 100000000;
    for(uint64_t i = 0; i < samples; ++i)
        z = sampler_1(&sc);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %fs\n", duration);
    printf("%.0f samples per seccond\n", samples / duration);

    return 0;
}