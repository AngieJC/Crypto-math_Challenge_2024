/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-05-06 22:34:47
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-12 23:21:20
 * @FilePath: /Crypto-math_Challenge_2024/test_acc.cpp
 */
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
extern "C" {
    #include "sampler.h"
    #include "my_sampler.h"
}
#include <cmath>
#include <iostream>

const int numSamples = 10000000; // 采样次数
double expectedMean; // 期望均值
double expectedStdDev; // 期望标准差
double MeanTolerance; // 均值误差误差
double StdDevTolerance; // 标准差误差
sampler_shake256_context rng;
sampler_context sc;

void sample_chara_check(int64_t sum, int64_t sumOfSquares) {
    double actualMean, actualVariance, actualStdDev;
    actualMean = static_cast<double>(sum) / numSamples;
    actualVariance = static_cast<double>(sumOfSquares) / numSamples - actualMean * actualMean;
    actualStdDev = sqrt(actualVariance);
    
    DOUBLES_EQUAL(expectedMean, actualMean, MeanTolerance);
    DOUBLES_EQUAL(expectedStdDev, actualStdDev, StdDevTolerance);
}

TEST_GROUP(SamplerTestGroup) {
    //
};

TEST(SamplerTestGroup, Sampler1DistributionTest) {
    expectedMean = 0.0;
    expectedStdDev = 0.75;
    MeanTolerance = 0.001;
    StdDevTolerance = 0.001;
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)"sampler_1", strlen("sampler_1"));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    int64_t sum = 0, sumOfSquares = 0;
    for(int i = 0; i < numSamples; ++i) {
        int sample = sampler_1(&sc);
        sum += sample;
        sumOfSquares += sample * sample;
    }
    sample_chara_check(sum, sumOfSquares);
}

TEST(SamplerTestGroup, Sampler2DistributionTest) {
    expectedMean = 0.0;
    expectedStdDev = 1024;
    MeanTolerance = 0.2;
    StdDevTolerance = 0.5;
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)"sampler_2", strlen("sampler_2"));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    int64_t sum = 0, sumOfSquares = 0;
    for(int i = 0; i < numSamples; ++i) {
        int sample = sampler_2(&sc);
        sum += sample;
        sumOfSquares += sample * sample;
    }
    sample_chara_check(sum, sumOfSquares);
}

TEST(SamplerTestGroup, Sampler3DistributionTest) {
    double expectedMeans[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.999999};
    expectedStdDev = 1.5;
    MeanTolerance = 0.001;
    StdDevTolerance = 0.001;
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)"sampler_3", strlen("sampler_3"));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    for(auto i = 0; i < 11; ++i) {
        int64_t sum = 0, sumOfSquares = 0;
        expectedMean = expectedMeans[i];
        for(int j = 0; j < numSamples; ++j) {
            int sample = sampler_3(&sc, expectedMean);
            sum += sample;
            sumOfSquares += sample * sample;
        }
        sample_chara_check(sum, sumOfSquares);
    }
}

TEST(SamplerTestGroup, Sampler4DistributionTest) {
    double expectedMeans[] = {0.0, 0.5, 0.999999};
    double expectedStdDevs[] = {0.800001, 1.2, 1.599999};
    MeanTolerance = 0.002;
    StdDevTolerance = 0.002;
    sampler_shake256_init(&rng);
    sampler_shake256_inject(&rng, (const uint8_t *)"sampler_4", strlen("sampler_4"));
    sampler_shake256_flip(&rng);
    Zf(prng_init)(&sc.p, &rng);

    for(auto i = 0; i < 3; ++i) {
        expectedMean = expectedMeans[i];
        for(auto j = 0; j < 3; ++j) {
            int64_t sum = 0, sumOfSquares = 0;
            expectedStdDev = expectedStdDevs[j];
            for(int k = 0; k < numSamples; ++k) {
                int sample = sampler_4(&sc, expectedStdDev, expectedMean);
                sum += sample;
                sumOfSquares += sample * sample;
            }
            sample_chara_check(sum, sumOfSquares);
        }
    }
}

int main(int argc, char **argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}