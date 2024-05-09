/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-05-06 22:34:47
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-09 15:05:10
 * @FilePath: /Crypto-math_Challenge_2024/my_sampler.h
 */
#include "sampler.h"

#ifndef INTEGER_GAUSSIAN_SAMPLER__
#define INTEGER_GAUSSIAN_SAMPLER__


/**************************************************************************
 ************************     API     *************************************
 **************************************************************************
 */
int sampler_1(void *ctx);

int sampler_2(void *ctx);

int sampler_3(void *ctx, double center);

int sampler_4(void *ctx, double sigma, double center);

uint8_t check_cnt(uint64_t *__restrict, uint64_t *__restrict, prng *__restrict);

// int sampler_base_3(prng *__restrict rng);

#endif