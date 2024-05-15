/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-04-13 11:22:48
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-15 14:14:15
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    printf("Test sampler %d: \n", sampler);
    fflush(stdout);

    clock_t start, end;
    uint32_t tsc_start_lo, tsc_start_hi, tsc_end_lo, tsc_end_hi;
    double duration;
    start = clock();
    __asm__ __volatile__("rdtsc" : "=a" (tsc_start_lo), "=d" (tsc_start_hi));
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const void *)seeds[sampler - 1], strlen(seeds[sampler - 1]));
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
    __asm__ __volatile__("rdtsc" : "=a" (tsc_end_lo), "=d" (tsc_end_hi));
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %fs\n", duration);
    printf("%.0f samples per seccond\n", samples / duration);
    printf("%zu clocks per sample\n", 
        (
            ((int64_t)tsc_end_lo | ((int64_t)tsc_end_hi) << 32) - 
            ((int64_t)tsc_start_lo | ((int64_t)tsc_start_hi) << 32)
        ) / samples);
    printf("Memory usage: %dKB\n", get_rmem(getpid()));

    return 0;
}