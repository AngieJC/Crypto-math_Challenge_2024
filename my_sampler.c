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
typedef int8_t v512i8 __attribute__ ((vector_size (512 / 8)));

// Fixed sigma = 0.75 and center = 0
int sampler_1(void *ctx){
    prng *restrict rng;
    int32_t d = 0, b = 0;
    static uint64_t b64 = 0, cnt = 0;
    rng = &((sampler_context *)ctx)->p;

    // const static uint8_t m1[5][64] = {
    //     {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
    //     {0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
    //     {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0}
    // };
    static const uint8_t m1_index[64][5] = {
        {0, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, 
        {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, {0, 1, 0, 0, 0}, {1, 3, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 2, 3, 0, 0}, {0, 2, 3, 0, 0}, {0, 1, 2, 3, 0}, 
        {1, 2, 0, 0, 0}, {1, 3, 0, 0, 0}, {1, 3, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 1, 2, 4, 0}, {0, 0, 0, 0, 0}, {0, 3, 4, 0, 0}, {1, 3, 4, 0, 0}, 
        {0, 4, 0, 0, 0}, {0, 1, 2, 3, 4}, {0, 1, 2, 4, 0}, {1, 0, 0, 0, 0}, {0, 1, 2, 3, 0}, {1, 2, 0, 0, 0}, {0, 4, 0, 0, 0}, {1, 0, 0, 0, 0}, 
        {0, 1, 2, 0, 0}, {1, 2, 3, 0, 0}, {1, 2, 4, 0, 0}, {2, 3, 4, 0, 0}, {0, 1, 4, 0, 0}, {0, 2, 3, 4, 0}, {1, 3, 0, 0, 0}, {1, 3, 0, 0, 0}, 
        {1, 0, 0, 0, 0}, {1, 2, 3, 4, 0}, {2, 4, 0, 0, 0}, {0, 4, 0, 0, 0}, {4, 0, 0, 0, 0}, {0, 1, 3, 4, 0}, {4, 0, 0, 0, 0}, {0, 1, 2, 3, 0}, 
        {0, 4, 0, 0, 0}, {2, 0, 0, 0, 0}, {0, 1, 2, 0, 0}, {3, 4, 0, 0, 0}, {0, 1, 2, 3, 4}, {3, 4, 0, 0, 0}, {0, 1, 2, 3, 4}, {0, 2, 3, 0, 0}, 
        {0, 1, 2, 3, 0}, {2, 3, 0, 0, 0}, {1, 4, 0, 0, 0}, {2, 3, 0, 0, 0}, {3, 0, 0, 0, 0}, {1, 3, 4, 0, 0}, {0, 3, 4, 0, 0}, {0, 1, 2, 0, 0}
    };
    static const uint8_t m1_col_sum[64] = {
        1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 3, 4, 2, 2, 2, 1, 4, 1, 3, 3, 2, 5, 4, 1, 4, 2, 2, 1, 3, 3, 3, 3, 3, 4, 2, 2, 1, 4, 2, 2, 1, 4, 1, 4, 2, 1, 3, 2, 5, 2, 5, 3, 4, 2, 2, 2, 1, 3, 3, 3
    };
    static const uint8_t col_index[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
        5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 8, 8
    };
    uint8_t b8 = prng_get_u8(rng);
    if(b8 < 254) {
        uint8_t whichcol = col_index[b8];
        d = m1_index[whichcol][m1_col_sum[whichcol] - ((b8 >> (7 - whichcol)) & 1) - 1];
        if(whichcol != 7)
            return (b8 & 1) ? d : -d;
        goto return_val;
    }
    d = b8 & 1;
    for(int col = 8; col < 64; ++col) {
        if(cnt == 0) {
            b64 = prng_get_u64(rng);
            cnt = 0x4000000000000000;
        }
        else
            cnt >>= 1;
        d = (d << 1) + (b64 & 1);
        b64 >>= 1;
        d -= m1_col_sum[col];
        if(d < 0) {
            d = m1_index[col][-(d + 1)];
            if(d == 0)
                return 0;
            return_val:
            if(cnt)
                cnt >>= 1;
            else {
                b64 = prng_get_u64(rng);
                cnt = 0x4000000000000000;
            }
            b = b64 & 1;
            b64 >>= 1;
            return b ? d : -d;
        }
    }
    return 0;
}

static inline void u64_to_v512ui(uint64_t x, v512u8 *v) {
    for(int i = 0; i < 64; ++i) {
        (*v)[i] = x & 1;
        x >>= 1;
    }
}

int sampler_1_SIMD(void *ctx){
    sampler_context *spc;
    spc = (sampler_context *)ctx;

    static const uint8_t m1_index[64][5] = {
        {0, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 0, 0, 0}, {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, 
        {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, {0, 1, 0, 0, 0}, {1, 3, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 2, 3, 0, 0}, {0, 2, 3, 0, 0}, {0, 1, 2, 3, 0}, 
        {1, 2, 0, 0, 0}, {1, 3, 0, 0, 0}, {1, 3, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 1, 2, 4, 0}, {0, 0, 0, 0, 0}, {0, 3, 4, 0, 0}, {1, 3, 4, 0, 0}, 
        {0, 4, 0, 0, 0}, {0, 1, 2, 3, 4}, {0, 1, 2, 4, 0}, {1, 0, 0, 0, 0}, {0, 1, 2, 3, 0}, {1, 2, 0, 0, 0}, {0, 4, 0, 0, 0}, {1, 0, 0, 0, 0}, 
        {0, 1, 2, 0, 0}, {1, 2, 3, 0, 0}, {1, 2, 4, 0, 0}, {2, 3, 4, 0, 0}, {0, 1, 4, 0, 0}, {0, 2, 3, 4, 0}, {1, 3, 0, 0, 0}, {1, 3, 0, 0, 0}, 
        {1, 0, 0, 0, 0}, {1, 2, 3, 4, 0}, {2, 4, 0, 0, 0}, {0, 4, 0, 0, 0}, {4, 0, 0, 0, 0}, {0, 1, 3, 4, 0}, {4, 0, 0, 0, 0}, {0, 1, 2, 3, 0}, 
        {0, 4, 0, 0, 0}, {2, 0, 0, 0, 0}, {0, 1, 2, 0, 0}, {3, 4, 0, 0, 0}, {0, 1, 2, 3, 4}, {3, 4, 0, 0, 0}, {0, 1, 2, 3, 4}, {0, 2, 3, 0, 0}, 
        {0, 1, 2, 3, 0}, {2, 3, 0, 0, 0}, {1, 4, 0, 0, 0}, {2, 3, 0, 0, 0}, {3, 0, 0, 0, 0}, {1, 3, 4, 0, 0}, {0, 3, 4, 0, 0}, {0, 1, 2, 0, 0}
    };
    static const uint8_t m1_col_sum[64] = {
        1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3, 3, 4, 2, 2, 2, 1, 4, 1, 3, 3, 2, 5, 4, 1, 4, 2, 2, 1, 3, 3, 3, 3, 3, 4, 2, 2, 1, 4, 2, 2, 1, 4, 1, 4, 2, 1, 3, 2, 5, 2, 5, 3, 4, 2, 2, 2, 1, 3, 3, 3
    };
    static uint8_t sample_cnt = 0;
    static int8_t samples[64] = {0};

    if(sample_cnt == 0) {
        sample_cnt = 63;
        v512u8 vcom = {0}, vWhichCol = {0}, vb = {0};
        v512i8 vd = {0};
        v512u8 vflag = {1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1, 
                        1, 1, 1, 1, 1, 1, 1, 1};
        for(int col = 0; col < 64; ++col) {
            u64_to_v512ui(prng_get_u64(&spc->p), &vb);
            vd = vd + vflag * (vd + vb);
            vcom = (vd >= m1_col_sum[col]) & 1;
            vd -= vflag * m1_col_sum[col];
            vflag &= vcom;
            vWhichCol += vflag;
        }
        u64_to_v512ui(prng_get_u64(&spc->p), &vb); // 优化：上面用过的随机数在这里再用一遍其实不会采样的随机性，所以这一行可以注释掉
        for(int i = 0; i < 64; ++i) {
            vd[i] = m1_index[vWhichCol[i]][-(vd[i] + 1)];
            samples[i] = vb[i] ? vd[i] : -vd[i];
        }
        return samples[0];
    }
    else
        return samples[sample_cnt--];
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