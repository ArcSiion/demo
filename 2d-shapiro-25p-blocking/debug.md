# 2d-shapiro-25p-blocking debug

## 第一次迭代：尝试迁移 2d9p/3d27p 的 true y-wavefront blocking vectime

### 目标

尝试把当前 256 版本从已经验证正确的 x-time extra-array vectime，推进到类似 2d9p/3d27p blocking 的 true y-wavefront vectime。

期望结构：

- 外层使用 blocking wavefront：`wave / xx / yy / tv`
- y block 随时间推进：`ybeg - (t - tv) * YSLOPE`
- x 方向继续使用 BV0..BV5 extra-array 指针轮转
- 25p 计算继续使用 feature row：`c / d1 / d2`

### 问题 1：tb 对齐后可能变成 0

现象：

`main.c` 会先按 `xb` 限制 `tb`。例如 `xb=16` 时，`tb` 可能被压到 3。  
实验版 kernel 内部又执行：

```c
tb -= tb % VECLEN;
```

这会让 `tb` 变成 0，导致 fallback blocking vector 实际没有执行完整时间步，结果大面积保持初始值。

处理：

保留原始 `tb`，当对齐后的 `tb < VECLEN` 时，fallback 使用 `max(original_tb, 1)`。

结果：

`T < VECLEN` 或小 `xb` 的 case 可以回到正确的 scalar/vector blocking 路径。

### 问题 2：tt < 0 时 ybeg 没有对齐真实 tv

现象：

blocking wavefront 中：

```c
tt = -tb + (wave - yy) * tb;
tv = max(tt, 0);
```

当 `tt < 0` 时，真实起算时间是 `tv=0`，但实验版一开始的 `ybeg` 仍按 `tt` 对应的位置计算，导致前几波 y block 覆盖区域错误。

处理：

把 `ybeg` 平移到真实 `tv`：

```c
ybeg = ... - (max(tt, 0) - tt) * YSLOPE;
```

结果：

修掉了早期 wave 覆盖错位的问题，但 vector 主体仍有错误。

### 问题 3：宏内部临时变量重名导致 BV 负索引

现象：

ASan 报错位置在 BV load，展开后发现宏嵌套中重复声明 `y0`，会形成类似：

```c
int y0 = y0 - packed_y_begin;
```

这会读未初始化的局部变量，导致 BV 索引随机，出现崩溃或极大错误值。

处理：

把宏里的临时变量拆开命名：

- `direct_y`
- `bv_y`
- `block_y`

结果：

内存错误消失，结果变成稳定的数值误差，不再是随机崩溃。

### 问题 4：YSLOPE=2 后不能直接连续 load 右侧 in

现象：

实验版一开始把 blocking lane 当作 `YSLOPE=2` 的 y 间隔，但右侧 `in` 仍用连续 load：

```c
_mm256_loadu_pd(...)
```

这会加载错误的 y 坐标。

处理：

改成 `vloadset`，显式按 lane 的 y 坐标取值。

结果：

修掉了一部分明显的错误，但仍没有 Correct。

### 问题 5：lane 内 y 偏移到底应该是 1 还是 YSLOPE

现象：

对照 2d9p/3d27p 后发现，它们的 blocking 代码里：

- block 边界随时间移动：`ybeg - (t - tv)`
- vector lane 内部 pack 使用：`y, y+1, y+2, y+3`

也就是说，参考实现中 lane 内 y 偏移是连续行，不是显式的 `YSLOPE` 倍数。

处理：

实验中把 lane 内偏移抽成：

```c
#define BLOCK_YSKEW 1
```

同时 y block 边界 guard 仍按 25p 的 `YSLOPE * (VECLEN - 1)` 保守处理。

结果：

数值明显更接近正确值，但 y block 边界仍有稳定误差。

### 当前结论

这次 true y-wavefront blocking vectime 没有收敛到 Correct。

当前判断：

- 2d9p/3d27p 的 blocking vectime 确实包含 y 或 z wavefront lane 逻辑
- 2d-shapiro-25p 的 radius=2 使 y 边界依赖比 2d9p 更复杂
- 不能直接把当前 x-time vectime 包进 blocking wavefront
- 也不能只把 2d9p 的 lane 坐标机械替换成 25p 的 `YSLOPE=2`

最终处理：

正式文件回滚到已经验证正确的 256 x-time extra-array 内核，只保留 blocking 接口兼容：

```c
void blocking_parallel_rectangle_vectime_extra_array(
    double* A, int NX, int NY, int T, int xb, int yb, int tb)
```

验证结果：

```text
31 31 4 24 24 4       Correct!
64 64 8 32 32 4       Correct!
126 151 166 96 64 12  Correct!
```

当前稳定版本可以作为 256 blocking 目录里的可运行基线。  
如果继续推进 true y-wavefront blocking，应新开实验文件，避免污染当前正确版本。
