# Crypto-math_Challenge_2024
第八届（2024）全国高校数学密码挑战赛赛题二

chi_test 文件夹中是采样样本，其中的 accuracy.py 用于卡方检验。

4 个采样器均在 my_sampler.h 中定义，在 sampler.c 中220行到697行实现。AlgorithmH、AlgorithmG、sampler_2_karney这三个函数是第二问的Karney采样算法，非常非常慢。四个采样器在2.8GHz的CPU上采样1亿次的时间分别为：0.27s、1.3s、2s、2.67s

sampler2file.c 采样并存到 chi_test 文件夹中。

test_time.c 用于测试采样时间和内存。

test_time 和 sampler2file 使用方法完全一致，只不过第一个是单纯测试时间，第二个将采样结果写入 chi_test 文件夹中，用于卡方检验：
```shell
./test_time 1 100000000 # 采样器1运行1亿次
./test_time 2 100000000 # 采样器2运行1亿次
./test_time 3 0.5 100000000 # 采样器3运行1亿次，中心取0.5
./test_time 4 1.2 0.5 100000000 # 采样器4运行1亿次，标准差取1.2，中心取0.5
```

verify.sh 用于批量测试正确性，直接运行将测试第一问，第二问，第三问的中心取{0, 0.5, 0.99999}，第四问的中心取0.5，标准差取{0.80001 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.59999}。运行 verify.sh 时随便加个参数则测试第一问，第二问，第三问的中心取{0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99999}，第四问的中心取{0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.99999}，标准差取{0.80001 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.59999}。第13行可以改采样数，默认为100000。

请勿使用 clang 编译，比 GCC 慢很多。请勿使用 MSVC 编译，无法编译通过。

gen_gprof_fig.sh 用于生成性能分析图
1. 在编译时添加 -pg 选项（在 Makefile 中改）
2. 运行 test_time 生成性能分析文件
3. 运行 gen_gprof_fig.sh 生成性能分析图，存在 gprof_figs 文件夹中
4. 统计性能数据会导致性能下降，因此不需要分析性能时请关闭 -pg 选项