/*
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-05-07 22:48:19
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-07 23:24:53
 * @FilePath: /Crypto-math_Challenge_2024/benchmark.c
 */
#include <math.h>
#include <time.h>
#include "dgs/dgs_gauss.h"

#define SAMPLES 100000000
#define METHOD_NUM 7

int main() {
    static const int methods[] = {
                     DGS_DISC_GAUSS_DEFAULT, 
                     DGS_DISC_GAUSS_ALIAS, 
                     DGS_DISC_GAUSS_CONVOLUTION, 
                     DGS_DISC_GAUSS_SIGMA2_LOGTABLE, 
                     DGS_DISC_GAUSS_UNIFORM_TABLE, 
                     DGS_DISC_GAUSS_UNIFORM_ONLINE, 
                     DGS_DISC_GAUSS_UNIFORM_LOGTABLE
                     };
    static const char *methods_name[] = {
                     "DGS_DISC_GAUSS_DEFAULT", 
                     "DGS_DISC_GAUSS_ALIAS", 
                     "DGS_DISC_GAUSS_CONVOLUTION", 
                     "DGS_DISC_GAUSS_SIGMA2_LOGTABLE", 
                     "DGS_DISC_GAUSS_UNIFORM_TABLE", 
                     "DGS_DISC_GAUSS_UNIFORM_ONLINE", 
                     "DGS_DISC_GAUSS_UNIFORM_LOGTABLE"
                     };
    
    // Sampler 1
    printf("************************************\n");
    printf("delta = 0.75, center = 0\n");
    clock_t start, end;
    for(size_t method = 0; method < METHOD_NUM; ++method) {
        start = clock();
        dgs_disc_gauss_dp_t *sampler = dgs_disc_gauss_dp_init(0.75, 0, 5, methods[method]);
        for(size_t i = 0; i < SAMPLES; ++i) {
            sampler->call(sampler);
        }
        end = clock();
        printf("Method: %s\nTime: %fs\n", methods_name[method], (double)(end - start) / CLOCKS_PER_SEC);
    }
}