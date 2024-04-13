from saga import parse_univariate_file, UnivariateSamples

sample1 = parse_univariate_file("sampler_1.txt")
mu, sigma, data = sample1[0][0], sample1[0][1], sample1[0][2]
res1 = UnivariateSamples(mu, sigma, data)
print("-----------------------------------------Sample 1-----------------------------------------", end = "")
print(res1)