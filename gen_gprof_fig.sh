#!/bin/bash
###
 # @Author: AngieJC htk90uggk@outlook.com
 # @Date: 2024-05-06 13:52:57
 # @LastEditors: AngieJC htk90uggk@outlook.com
 # @LastEditTime: 2024-05-06 14:27:32
 # @FilePath: /Crypto-math_Challenge_2024/gen_gprof_fig.sh
### 

TIME=$(date +"%Y_%m_%d_%H_%M_%S")
FILENAME=./gprof_figs/test_time_${TIME}.eps

gprof test_time gmon.out | gprof2dot | dot -Teps -o "$FILENAME"
