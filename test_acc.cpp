#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
extern "C" {
    #include "sampler.h"
    #include "my_sampler.h"
}
#include <cmath>

const int numSamples = 100000000; // 采样次数
const double tolerance = 0.001; // 误差
double expectedMean; // 期望均值
double expectedStdDev; // 期望标准差
sampler_shake256_context rng;
sampler_context sc;

void sample_chara_check(int64_t sum, int64_t sumOfSquares) {
    double actualMean, actualVariance, actualStdDev;
    actualMean = static_cast<double>(sum) / numSamples;
    actualVariance = static_cast<double>(sumOfSquares) / numSamples - actualMean * actualMean;
    actualStdDev = sqrt(actualVariance);
    
    DOUBLES_EQUAL(expectedMean, actualMean, tolerance);
    DOUBLES_EQUAL(expectedStdDev, actualStdDev, tolerance);
}

TEST_GROUP(SamplerTestGroup) {
    //
};

TEST(SamplerTestGroup, Sampler1DistributionTest) {
    expectedMean = 0.0;
    expectedStdDev = 0.75;
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
    expectedStdDev = 0;
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

int main(int argc, char **argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}