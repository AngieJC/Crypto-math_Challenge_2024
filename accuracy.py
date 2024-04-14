import sys
from saga import parse_univariate_file, UnivariateSamples

if len(sys.argv) == 1:
    exit("need file name")
sample1 = parse_univariate_file(sys.argv[1])
mu, sigma, data = sample1[0][0], sample1[0][1], sample1[0][2]
res1 = UnivariateSamples(mu, sigma, data)
print(res1)