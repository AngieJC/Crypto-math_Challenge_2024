<!--
 * @Author: AngieJC htk90uggk@outlook.com
 * @Date: 2024-05-10 22:59:19
 * @LastEditors: AngieJC htk90uggk@outlook.com
 * @LastEditTime: 2024-05-12 17:24:26
 * @FilePath: /Crypto-math_Challenge_2024/todo.md
-->
# To-Do List

- [x] 第一题测试SIMD并行化：平均内存访问超过两次，采样速率无法提升
- [x] 测试幂运算的递归和循环展开速度：递归指令略多，读数据较少，写数据略多，安心用循环算了
- [x] 构建好用的离散半高斯分布：离散半高斯分布就是离散高斯分布去掉负半轴再归一化
- [x] 测试exp(x)的最低精度：泰勒展开到5次仍可通过检测，保险起见，保留到6次
- [ ] 遍历目标列那里-(d + 1)可以改成-d，只需稍微修改一下矩阵
- [ ] 研究为什么第四题改成标准差为1.6的离散半高斯分布后不能通过检测