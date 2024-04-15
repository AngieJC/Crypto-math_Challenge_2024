#!/bin/bash

TIME=$(date +"%Y_%m_%d_%H_%M_%S")
FILENAME=./gprof_figs/test_time_${TIME}.pdf

gprof test_time gmon.out | gprof2dot -n0 -e0 | dot -Tpdf -o "$FILENAME"
