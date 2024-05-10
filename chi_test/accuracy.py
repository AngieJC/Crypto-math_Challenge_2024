'''
Author: AngieJC htk90uggk@outlook.com
Date: 2024-05-06 22:34:47
LastEditors: AngieJC htk90uggk@outlook.com
LastEditTime: 2024-05-10 09:09:34
FilePath: /Crypto-math_Challenge_2024/chi_test/accuracy.py
'''
import sys
from saga import parse_univariate_file, UnivariateSamples

if len(sys.argv) == 1:
    exit("need file name")
sample = parse_univariate_file(sys.argv[1])
mu, sigma, data = sample[0][0], sample[0][1], sample[0][2]
res = UnivariateSamples(mu, sigma, data)
print(res)