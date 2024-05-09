/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-05-06 22:34:47
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-09 11:51:23
 * @FilePath: /Crypto-math_Challenge_2024/sampler_4.c
 */
#include "my_sampler.h"

// sigma is uniformly distributed over (0.8,1.6) and center is uniformly distributed over [0,1)
int sampler_4(void *ctx, double sigma, double center){
    int z = 0;
    prng *restrict rng = &((sampler_context *)ctx)->p;

    return z;
}
