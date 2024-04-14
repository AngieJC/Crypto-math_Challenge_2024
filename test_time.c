/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-04-13 11:22:48
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-04-13 11:39:57
 */
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

    printf("Test sampler: \n");
    fflush(stdout);

    clock_t start, end;
    double duration;
    start = clock();
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)seeds[sampler], strlen(seeds[sampler]));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    switch (sampler)
    {
    case 1:
        for(uint64_t i = 0; i < samples; ++i)
            sampler_1(&sc);
        break;
    case 2:
        for(uint64_t i = 0; i < samples; ++i)
            sampler_2(&sc);
        break;
    case 3:
        for(uint64_t i = 0; i < samples; ++i)
            sampler_3(&sc, mu);
        break;
    case 4:
        for(uint64_t i = 0; i < samples; ++i)
            sampler_4(&sc, sigma, mu);
        break;
    default:
        break;
    }
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %fs\n", duration);
    printf("%.0f samples per seccond\n", samples / duration);

    return 0;
}