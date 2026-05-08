2d-shapiro-25p vectime notes

这版 vectime.c 尽量按 2d-jacobi-9p/vectime.c 的原地转置逻辑迁移。
接口和 define.h 里的 Compute_scalar / Compute_1vector 都不改。

时间向量仍然是 4 个 lane。
对于 base x 和 y，向量表示：
B[t + 1][x][y]
B[t][x + STRIDE][y]
B[t + 1][x + 2 * STRIDE][y]
B[t][x + 3 * STRIDE][y]

Shapiro 是 radius 2，所以每个 x 主体需要 5 列：
x - 2, x - 1, x, x + 1, x + 2。
初始阶段把这 5 列按 2d9p 的 4x4 transpose 方案原地转置到 B 里。
主体每前进一个 x，就用当前输出 rotate/blend 生成新的右列 x + STRIDE。
结束时把最后留下的 5 列再 transpose 回普通布局。

每个时间 tile 仍然分三段：
左侧 scalar 预热阶梯区。
中间 temporal-vector 主体。
右侧 scalar 收尾阶梯区。

y 方向满 4 个点的部分使用原地转置布局。
y 尾部不再转置，直接用 gather/scatter 兜底。

25p 计算现在按当前 yy 拆成三阶段：
阶段 1 加载 y - 2 层并准备 C02/C22/C12 的部分和。
阶段 2 加载 y - 1, y, y + 1 层并补齐 C01/C11/中心项。
阶段 3 加载 y + 2 层，按原 Compute_1vector 的系数组顺序生成 vout。
这里没有使用 BV_ACC，也没有把中间结果写回内存。
每个 yy 算完立刻执行 Input_Output，避免改变 2d9p 的依赖顺序。

当前目标是让 25p 的迁移逻辑贴近 2d9p，并保证 Correct!。
暂时不考虑寄存器压力，也不追求最终性能。
