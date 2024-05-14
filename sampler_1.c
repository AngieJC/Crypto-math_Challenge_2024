/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-05-08 09:16:03
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-14 10:37:06
 * @FilePath: /Crypto-math_Challenge_2024/sampler_1.c
 */
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

uint8_t check_cnt(uint64_t *__restrict cnt, uint64_t *__restrict b64, prng *__restrict rng) {
    if(*cnt)
        (*cnt) >>= 1;
    else {
        *b64 = prng_get_u64(rng);
        *cnt = 0x4000000000000000;
    }
    uint8_t b = (*b64) & 1;
    (*b64) >>= 1;
    return b;
}

// Fixed sigma = 0.75 and center = 0
int sampler_1(void *ctx){
    static const uint8_t m1_index[16][5] = {
        {1, 2, 0, 0, 0}, {1, 2, 0, 0, 0}, {0, 1, 0, 0, 0}, {1, 3, 0, 0, 0}, 
        {0, 0, 0, 0, 0}, {1, 2, 3, 0, 0}, {0, 2, 3, 0, 0}, {0, 1, 2, 3, 0}, 
        {1, 2, 0, 0, 0}, {1, 3, 0, 0, 0}, {1, 3, 0, 0, 0}, {1, 0, 0, 0, 0}, 
        {0, 1, 2, 4, 0}, {0, 0, 0, 0, 0}, {0, 3, 4, 0, 0}, {1, 3, 4, 0, 0}
    };
    static const uint8_t m1_col_sum[16] = {
        2, 2, 2, 2, 1, 3, 3, 4, 
        2, 2, 2, 1, 4, 1, 3, 3
    };
    static const int8_t sample_val[254] = {
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
        1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
        1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , -1, -1, -1, -1, 
        1 , 1 , -1, -1, 2 , 2 , -2, -2, 1 , -1, 2 , -2, 1 , 2
    };

    prng *__restrict rng = &((sampler_context *)ctx)->p;
    static uint64_t b64 = 0, cnt = 0;
    int32_t d;
    uint8_t b8 = prng_get_u8(rng);
    if(__glibc_likely(b8 < 252))
        return sample_val[b8];
    if(b8 < 254)
        return check_cnt(&cnt, &b64, rng) ? sample_val[b8] : -sample_val[b8];
    b8 = ~b8;
    d = b8;
    #if defined __GNUC__
    #pragma GCC unroll 16
    #elif defined __clang__
    #pragma clang loop unroll(full)
    #endif
    for(int col = 0; col < 16; ++col) {
        d = (d << 1) + check_cnt(&cnt, &b64, rng) - m1_col_sum[col];
        if(d < 0) {
            d = m1_index[col][-(d + 1)];
            return check_cnt(&cnt, &b64, rng) ? d : -d;
        }
    }
    return 0;
}
