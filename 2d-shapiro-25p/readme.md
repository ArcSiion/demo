2d-shapiro-25p vectime notes

这版 vectime.c 借鉴 3d-jacobi-27p 的 extra-array 思路。
接口和 define.h 里的 Compute_scalar / Compute_1vector 都不改。

时间向量仍然是 4 个 lane。
对于 base x 和 y，向量内容沿用 2d9p 的布局：
B[t + 1][x][y]
B[t][x + STRIDE][y]
B[t + 1][x + 2 * STRIDE][y]
B[t][x + 3 * STRIDE][y]

Shapiro 是 radius 2，所以每个 x 主体需要 5 个输入 slice：
x - 2, x - 1, x, x + 1, x + 2。
这 5 个输入 slice 打包到 BV0..BV4。
BV5 是当前 x 生成的新右 slice。
x 循环结束后做指针轮转：
BV0 <- BV1 <- BV2 <- BV3 <- BV4 <- BV5 <- old BV0。

每个时间 tile 仍然分三段：
左侧 scalar 预热阶梯区。
中间 temporal-vector 主体。
右侧 scalar 收尾阶梯区。

y 方向分成三段：
y 低边界 chunk 使用 boundary loader。
y 主路径所有 stencil 行都在 BV 内，使用无分支 BV loader。
y 高边界和 y tail 使用 boundary/direct 路径。

25p 计算现在按 y 方向复用 feature row 窗口。
一个 raw row 原本有 5 个向量：
m2, m1, c, p1, p2。
现在加载时先聚合成 3 个特征：
c  = x
h1 = x - 1 + x + 1
h2 = x - 2 + x + 2

一个 y chunk 先装入 5 行 row0..row4。
计算完一个 yy 后，row0..row4 左移一行，只加载下一行。
这样 4 个连续 yy 需要加载 y - 2 到 y + 5 共 8 行。
每行只保留 c/h1/h2，所以窗口从 25 个 vec 降到 15 个 vec。

当前没有 BV_ACC，也没有把 partial sum 写回内存。
取消了旧版 pending 队列，新右 slice 直接写入 BV5。

当前目标是借鉴 3d27p 的 BV 窗口轮转方式，并尝试降低寄存器压力。
feature row 会改变部分加法结合顺序。
main.c 的 checkresult 参考 3d27p 改成了相对误差容差判断。
Makefile 打开了 FMA 编译路径。
性能还需要继续观察和微调。
