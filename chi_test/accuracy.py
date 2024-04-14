import sys
from saga import parse_univariate_file, UnivariateSamples

if len(sys.argv) == 1:
    exit("need file name")
sample = parse_univariate_file(sys.argv[1])
mu, sigma, data = sample[0][0], sample[0][1], sample[0][2]
res = UnivariateSamples(mu, sigma, data)
print(res)