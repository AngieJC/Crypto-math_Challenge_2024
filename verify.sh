###
 # @Author: AngieJC htk90uggk@outlook.com
 # @Date: 2024-05-15 11:20:25
 # @LastEditors: AngieJC htk90uggk@outlook.com
 # @LastEditTime: 2024-05-15 14:21:52
 # @FilePath: /Crypto-math_Challenge_2024/verify.sh
### 
#!/bin/bash

# activate the virtual environment
echo "MUST ACTIVATE THE VIRTUAL ENVIRONMENT FIRST!"

times=100000000

# sampler_1
filename="sampler_1_$times.txt"
echo "************** sampler 1 **************"
./sampler2file 1 $times
cd chi_test
python accuracy.py $filename
rm $filename
cd ..
echo

# sampler_2
filename="sampler_2_$times.txt"
echo "************** sampler 2 **************"
./sampler2file 2 $times
cd chi_test
python accuracy.py $filename
rm $filename
cd ..
echo

# sampler_3
filename="sampler_3_$times.txt"
if [ $# -eq 0 ]; then
    centers=(0 0.5 0.99999)
else
    centers=(0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99999)
fi
echo "************** sampler 3 **************"
for center in ${centers[@]}; do
    echo "--- center: $center ---"
    ./sampler2file 3 $center $times
    cd chi_test
    python accuracy.py $filename
    cd ..
    echo
done
cd chi_test
rm $filename
cd ..

# sampler_4
filename="sampler_4_$times.txt"
if [ $# -eq 0 ]; then
    centers=(0.5)
fi
devs=(0.80001 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.59999)
echo "************** sampler 4 **************"
for dev in ${devs[@]}; do
    for center in ${centers[@]}; do
        echo "--- dev: $dev, center: $center ---"
        ./sampler2file 4 $dev $center $times
        cd chi_test
        python accuracy.py $filename
        cd ..
        echo
    done
done
cd chi_test
rm $filename
cd ..
