#include "my_sampler.h"

/*
 *
 * ********************************Attention!!!************************************************
 * ctx : Pointer to the sampler_context structure, including PRNG or other.
 * ********************************************************************************************
 * Please read the following precautions carefully!!!!
 * 1. In supplied folder "example/sampler", it offers a falcon's sampler as an instance for giving the codes about pointer "ctx".
 * 2. In folder "example/sampler", it includes the implementation of SHAKE256 in shake.c and PRNG in rng.c, please just use it.
 * 3. In the head file "example/sampler/sampler.h", it gives the definition of "ctx" in the struct "sampler_context", please just use it and don't change it.
 * 4. In the main file "example/sampler/main.c", it provides the procedure of how to initialize the pointer ctx, you should learn it and use it.
 * 5. The API of sampler function in the "example/sampler" is similar to ours. Pointer ctx is the same and fpr also denotes the type of "double", but we use "double" here.
 *
 *
 *In conclusion, you should use the method of defining and initializing for pointer ctx in given "example/sampler",
 *please don't change it and just use it. You should focus on the four samplers themselves. Pointer ctx should be the same for everyone.
 *
 *
 * */


typedef uint8_t v512u8 __attribute__ ((vector_size (512 / 8)));
typedef uint8_t v512i8 __attribute__ ((vector_size (512 / 8)));

// Fixed sigma = 0.75 and center = 0
int sampler_1(void *ctx){
    sampler_context *spc;
    int32_t d = 0, b = 0;
    static uint64_t bs = 0, cnt = 0;
    spc = (sampler_context *)ctx;

    const static uint8_t m1[5][64] = {
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
        {0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0}
    };
    const static uint8_t m1_col_sum[64] = {
        1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 3, 4, 2, 2, 2, 1, 4, 1, 3, 3, 2, 5, 4, 1, 4, 2, 2, 1, 3, 3, 3, 3, 3, 4, 2, 2, 1, 4, 2, 2, 1, 4, 1, 4, 2, 1, 3, 2, 5, 2, 5, 3, 4, 2, 2, 2, 1, 3, 3, 3
    };
    for(int col = 0; col < 64; ++col) {
        if(cnt == 0) {
            bs = prng_get_u64(&spc->p);
            cnt = 0x4000000000000000;
        }
        else
            cnt >>= 1;
        d = (d << 1) + (bs & 1);
        bs >>= 1;
        d -= m1_col_sum[col];
        if(d < 0) {
            for(int row = 0; row < 5; ++row) {
                d += m1[row][col];
                if(d == 0) {
                    if(row == 0)
                        return row;
                    if(cnt == 0) {
                        bs = prng_get_u64(&spc->p);
                        cnt = 0x4000000000000000;
                    }
                    else
                        cnt >>= 1;
                    b = bs & 1;
                    bs >>= 1;
                    return b ? row : -row;
                }
            }
        }
    }
    return 0;
}

// Fixed sigma = 1024 and center = 0
int sampler_2(void *ctx){
    double sigma = 1024;
    double center = 0;

    int z = 0;


    return z;

}

// Fixed sigma = 1.5 and center c is uniformly distributed over [0,1)
int sampler_3(void *ctx, double center){
    double sigma = 1.5;

    int z = 0;


    return z;


}

// sigma is uniformly distributed over (0.8,1.6) and center is uniformly distributed over [0,1)
int sampler_4(void *ctx, double sigma, double center){

    int z = 0;


    return z;

}