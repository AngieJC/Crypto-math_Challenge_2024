# Crypto-math_Challenge_2024
第八届（2024）全国高校数学密码挑战赛赛题二

chi_test 文件夹中是采样样本，其中的 accuracy.py 用于卡方检验。

4 个采样器均在 my_sampler.h 中定义，在 sampler_[i].c 中实现。

sampler2file.c 采样并存到 chi_test 文件夹中。

test_time.c 用于测试采样时间和内存。

test_acc.cpp 主要检测采样均值和标准差是否符合给定的值，只用于粗略检验，严格检验还需要卡方验证。依赖库：cpputest

请勿使用 clang 编译，比 GCC 慢很多，而且 sampler_1.c 中使用了 GCC 的扩展语法，其他编译器编译不通过。

gen_gprof_fig.sh 用于生成性能分析图
1. 在编译时添加 -pg 选项（在 Makefile 中改）
2. 运行 test_time 生成性能分析文件
3. 运行 gen_gprof_fig.sh 生成性能分析图，存在 gprof_figs 文件夹中
4. 统计性能数据会导致性能下降，因此不需要分析性能时请关闭 -pg 选项