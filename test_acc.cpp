#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
extern "C" {
    #include "sampler.h"
    #include "my_sampler.h"
    #include "stdio.h"
}
#include <cmath>

TEST_GROUP(SamplerTestGroup) {
    //
};

TEST(SamplerTestGroup, Sampler1DistributionTest) {
    const int numSamples = 100000000; // 采样次数
    const double expectedMean = 0.0; // 期望均值
    const double expectedStdDev = 0.75; // 期望标准差
    const double tolerance = 0.001; // 误差
    sampler_shake256_context rng;
    sampler_context sc;
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

    // 样本特征
    double actualMean = static_cast<double>(sum) / numSamples;
    double actualVariance = static_cast<double>(sumOfSquares) / numSamples - actualMean * actualMean;
    double actualStdDev = sqrt(actualVariance);

    // 检查
    DOUBLES_EQUAL(expectedMean, actualMean, tolerance);
    DOUBLES_EQUAL(expectedStdDev, actualStdDev, tolerance);
}

int main(int argc, char **argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}