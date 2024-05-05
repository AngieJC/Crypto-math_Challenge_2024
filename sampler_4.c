#include "my_sampler.h"

// sigma is uniformly distributed over (0.8,1.6) and center is uniformly distributed over [0,1)
int sampler_4(void *ctx, double sigma, double center){
    int z = 0;
    prng *restrict rng = &((sampler_context *)ctx)->p;

    return z;
}
