/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-04-30 15:26:56
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-15 14:18:58
 * @FilePath: /Crypto-math_Challenge_2024/sampler2file.c
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
    const char *seeds[4] = {"sampler_1", "sampler_2", "sampler_3", "sampler_4"};
    char file_name[1024] = {0};
    char first_line[1024] = {0};

    clock_t start, end;
    double duration;
    start = clock();
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const void *)(seeds[sampler - 1]), strlen(seeds[sampler - 1]));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    sprintf(file_name, "./chi_test/sampler_%d_%lu.txt", sampler, samples);
    printf("Write samples to file: %s\n", file_name);
    fflush(stdout);
    FILE *f = fopen(file_name, "w");
    sprintf(first_line, "mu = %f, sigma = %f\n", mu, sigma);
    fprintf(f, "%s", first_line);
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