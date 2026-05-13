# 2d-shapiro-25p-blocking debug

## 第十三次迭代：big-test 改为可配置容差并输出误差摘要（保留）

### 问题

超大规模测试：

```text
6144 4096 36000 1536 1024 256
```

性能正常，但校验报了 3 个 mismatch：

```text
Correct = 0.498681056666718, Wrong = 0.498681056667917
Correct = 0.136749152043461, Wrong = 0.136749152044628
Correct = 0.635350032770353, Wrong = 0.635350032769278
```

这些点的误差约为 `1.1e-12 ~ 1.2e-12`，只是略高于 big-test 中写死的 `1e-12` relative tolerance。结合 warmup Correct 和 mismatch 数量极少，判断为长时间步下浮点重排/FMA 产生的舍入差异，不是算法边界错误。

### 修改方法

只修改 big-test 校验入口，不改核心算法：

- `main_big.c` 增加 `BIG_CHECK_RTOL` 和 `BIG_CHECK_ATOL`
- 默认使用 `rtol=1e-10, atol=1e-12`
- 仍然统计 strict `1e-12` mismatch 数
- 输出 `max_abs/max_rel` 和最大误差坐标
- 输出当前使用的容差和 `tolerance_mismatch`

这样可以同时保留严格误差信息和实际长测 Correct 判定。

### 测试结果

smoke test 通过：

```text
31 31 4 24 24 4
Check summary: max_abs = 0.000e+00, max_rel = 0.000e+00
Check tolerance: atol = 1.000e-12, rtol = 1.000e-10, strict_1e-12_mismatch = 0, tolerance_mismatch = 0
Correct! blocking_parallel_rectangle_vectime_extra_array
```

对用户已跑的超大规模日志，新的默认容差会把 `1.2e-12` 量级差异归为 Correct，同时仍会报告 strict `1e-12` 下的 mismatch 数。

### 是否保留

保留。

原因：

- 不改变算法和性能
- 长时间步测试更符合浮点误差现实
- 输出保留严格误差统计，便于后续判断是否出现真正错误

## 第十二次迭代：限制 vectime OpenMP 线程数到实际 owner block 数（保留）

### 问题

第十一次已经复用了 `LB/AV` 缓冲，但目标参数：

```text
4125 2125 1162 1536 1024 256
```

只有 `ceil(NX / xb) * ceil(NY / yb) = 9` 个 owner block。默认环境有 112 个 OpenMP 线程，vectime wrapper 每个时间块会启动远多于实际任务数的线程，产生明显调度和同步开销。

### 修改方法

在 vectime wrapper 中计算实际 block 数：

```c
xblocknum = myceil(NX, xb)
yblocknum = myceil(NY, yb)
threadnum = min(omp_get_max_threads(), xblocknum * yblocknum)
```

并在主并行循环上使用：

```c
num_threads(threadnum)
```

同时 `LB_pool/AV_pool` 也按这个线程数分配。

该修改不改变算法、不改变分块范围、不改变浮点计算顺序。

### 测试结果

基础规模全部 Correct：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.000125

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.002664

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.600554
```

目标大规模 Correct，且相对第十一次明显提升：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.885288
```

### 是否保留

保留。

原因：

- Correctness 通过
- 不改变核心内核语义
- 中等规模和目标大规模都有明显收益
- 目标大规模已经显著高于 blocking scalar/vector

当前不足：

- 很小规模因为强制走分块路径，计时仍被 OpenMP 和局部缓冲开销主导
- 当前仍是 local-buffer blocking，不是真正的 2d9p wavefront lane 版本
- 后续若继续优化，重点应放在减少 halo 重算或探索更稳妥的 wavefront 迁移

## 第十一次迭代：复用线程私有 LB/AV 缓冲并用行级 memcpy 拷贝（保留）

### 问题

第十次 x/y owner block 已经 Correct，且目标大规模性能较好，但 wrapper 仍有明显额外开销：

- 每个时间块重新分配 `SRC`
- 每个 owner block 分配/释放局部 `LB`
- 每次调用 full-domain local kernel 都分配/释放 AV extra-array
- 局部缓冲初始化和 owner 回写使用逐元素循环

这些开销不改变算法结果，但会放大小/中规模的分块成本。

### 修改方法

保持算法和浮点计算顺序不变，只调整内存管理和拷贝方式：

- `SRC` 移到时间循环外，只分配一次并在每个时间块复用
- 增加 `LB_pool[thread]`，每个 OpenMP 线程懒分配一个最大局部缓冲
- 增加 `AV_pool[thread]`，full-domain local kernel 复用线程私有 AV extra-array
- `SRC` 快照使用一次整块 `memcpy`
- `LB` 初始化和 owner 回写改成按行 `memcpy`

该版本仍然是 x/y owner block local-buffer wrapper，不改变 lane 布局，也不引入 true y-wavefront lane。

### 测试结果

小/中规模全部 Correct。相对第十次，`126x151x166` 有明显恢复：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.058242

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.163840

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.325331
```

目标大规模 Correct：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.453677
```

### 是否保留

保留。

原因：

- Correctness 通过
- 不改变核心 25p 时间向量计算
- 中等规模性能明显改善
- 目标大规模仍保持高于 blocking scalar/vector 的优势

当前不足：

- 大规模结果低于第十次记录的 `1.511587`，需要后续重复测试确认波动范围
- wrapper 仍有 halo 重算成本，本质上还不是 2d9p 的 wavefront blocking
- 继续提升需要减少 local-buffer wrapper 的重复计算，或者谨慎迁移 true blocking 结构

## 第十次迭代：x/y owner block 局部缓冲分块（保留观察）

### 问题

第九次虽然保证所有规模都走分块路径，但分块仍只在 x 方向发生，`yb` 没有进入核心计算。  
这不符合当前目标：风格上应更接近其它 2D blocking 目录，至少要让 x/y owner block 都参与。

### 修改方法

把 wrapper 从 x-only owner block 改成 x/y owner block：

- 外层按 `block_t` 切时间块
- 每个时间块复制 source parity 到 `SRC`
- owner 区域按 `xb` 和 `yb` 同时切分
- 每个 owner block 拷贝局部 `LB`
- x halo 使用 `XSLOPE * dt`
- y halo 使用 `YSLOPE * dt`
- 在局部 `LB` 上调用已验证的 full-domain x-time extra-array 内核
- 只回写 owner x/y 区域

该版本仍然不改变 lane 布局：

- lane 仍是连续 y：`y, y+1, y+2, y+3`
- 不做 `y,y+2,y+4,y+6`
- 不做 even/odd phase split
- 不引入 true y-wavefront lane

### 测试结果

小/中规模全部 Correct，但局部缓冲拷贝和 halo 重算开销明显，性能比第九次 x-only wrapper 更低：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.059138

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.172463

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.199679
```

目标大规模测试 Correct，且相对第八/九次记录有明显提升：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.511587
```

### 是否保留

暂时保留观察。

原因：

- 满足所有规模都走分块路径
- `xb/yb/tb` 都进入正式路径
- Correctness 覆盖小/中/目标大规模
- 目标大规模性能目前最好

当前不足：

- 小/中规模性能明显下降
- 仍然不是 2d9p 那种 wavefront lane 版本，而是 overlapped local-buffer blocking
- 每个 block 都调用 full-domain local kernel，局部缓冲分配/拷贝开销重
- 大规模结果需要再重复几次确认稳定性

下一步：

- 重复目标大规模，确认 `1.5 GStencil/s` 是否稳定
- 尝试减少每个 block 的 `malloc/free`
- 尝试复用每线程局部缓冲
- 评估是否可以去掉每个时间块的全局 `SRC` 完整复制

## 第九次迭代：去掉规模门槛，所有 case 都走分块路径

### 问题

第八次正式版本为了避免小/中规模局部缓冲开销，在 wrapper 中设置了门槛：

```c
work < 1e9 || block_t < 64 || NX <= xb
```

满足这些条件时直接回到 full-domain x-time 内核。这样虽然性能更稳，但不符合当前目标：`2d-shapiro-25p-blocking` 应该始终走分块路径，风格上更接近其它 blocking 目录，而不是按规模退回非分块路径。

### 修改方法

删除正式函数中的规模门槛：

```c
if (xb <= 0 || work < 1000000000LL || block_t < 64 || NX <= xb) {
    shapiro_vectime_full_domain(...);
    return;
}
```

保留必要的参数保护：

```c
if (xb <= 0) {
    xb = NX;
}
```

现在所有规模都会进入 x-block local-buffer wrapper：

- 按 `block_t = tb - tb % VECLEN` 切时间块
- 每个时间块复制 source parity 到 `SRC`
- 每个 x owner block 拷贝局部 `LB`
- 在局部 `LB` 上调用已验证的 x-time extra-array 内核
- 只回写 owner x 区间

### 测试结果

基础测试全部 Correct：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.071185

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.179060

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.368403
```

### 是否保留

暂时保留。

原因：

- 满足“所有规模都走分块路径”的要求
- Correctness 通过
- 代码入口不再按规模隐藏 fallback

当前不足：

- 该路径仍然只是 x-block local-buffer，不是真正的 2d9p 风格 y-wavefront blocking
- `yb` 仍没有进入核心计算
- 小/中规模性能明显低于之前 fallback 版本，说明局部缓冲和拷贝开销较重

下一步应继续把结构往 2d9p blocking 风格靠拢，但仍避免高风险的 `y,y+2,y+4,y+6` lane 重写。

## 第八次迭代：x-block 局部缓冲时间块 wrapper（保留）

### 问题

前几次 y 方向分块要么破坏时间状态顺序，要么 Correct 但性能明显下降。继续保持当前 256 时间向量组成不变，改从 x 方向做 owner block：每个时间块只在局部 x 区间内调用已验证的 full-domain x-time 内核，再回写 owner 区域。

### 修改方法

正式函数拆成两层：

- `shapiro_vectime_full_domain`：原已验证 x-time extra-array 内核，保持 `Input_Output_1..4`、连续 y lane、`BV0..BV5` 轮转不变
- `blocking_parallel_rectangle_vectime_extra_array`：大规模时启用 x-block wrapper，小/中规模直接 fallback 到 full-domain 内核

x-block wrapper 策略：

- 每个时间块 `dt = min(block_t, T - tt)`，其中 `block_t = tb - tb % VECLEN`
- 先复制当前 source parity 到 `SRC` 快照，避免 `dt` 为偶数时 owner 回写覆盖其它 block 的输入 halo
- 每个 x owner block 拷贝 `[owner_begin - XSLOPE * dt, owner_end + XSLOPE * dt)` 的局部缓冲
- 局部缓冲两个 parity 都初始化为同一 source 快照
- 调用 `shapiro_vectime_full_domain((double *)LB, local_NX, NY, dt, ...)`
- 只把 owner x 区间、真实 y 区间的 final parity 回写到全局数组
- 当前保留门槛：`work >= 1e9`、`block_t >= 64`、`NX > xb`；否则走 full-domain fallback，避免小规模拷贝回退

### 测试结果

小/中规模走 fallback，保持 Correct，避免局部缓冲版本在这些规模上的回退：

```text
64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.321255

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.441167
```

实验入口直接对比时，x-block 局部缓冲在中等规模仍慢，但目标大规模 Correct 且明显更快：

```text
126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.442217
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.255362

4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.080952
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 1.389238
```

提升为正式版本并删除实验入口后，大规模 x-block path 重复验证：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.180110

4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.119085
```

同一正式二进制用 `tb=4` 强制走 full-domain fallback，作为旧路径参考：

```text
4125 2125 1162 1536 1024 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.004781
```

说明大规模 x-block path 在同机同构建下相对 fallback 有稳定收益。

### 是否保留

保留。

原因：

- 不改变时间向量组成，不引入 y-skew
- Correctness 覆盖小/中/大规模
- 大规模目标参数下相对旧 full-domain 路径有稳定收益
- 小/中规模通过门槛继续走旧路径，避免局部缓冲拷贝导致回退

## 第七次迭代：单 x 内 y-vector 并行实验（不保留）

### 问题

第六次把 y block 外提成独立 x 扫描后 Wrong，说明不能改变全局 x-sweep 的时间状态顺序。为了继续保留当前时间向量组成，尝试只在每个固定 x 上并行分担 y 向量块：x 仍严格顺序推进，`BV0..BV5` 仍按原顺序轮转。

### 修改方法

新建实验入口 `blocking_parallel_rectangle_vectime_extra_array_exp`：

- 保持 `Input_Output_1..4` 不变
- 保持连续 y lane 和 `BV0..BV5` 轮转不变
- 每个 x 位置上用 `#pragma omp parallel` + `#pragma omp for schedule(static)` 分担完整 y vector chunks
- 每个 y vector chunk 独立从 `BV0..BV4` 或边界 `B` 读取 `y-2..y+5`
- 尾部 y rows 仍由单线程走正式版本的 direct gather/scatter

### 测试结果

Correctness 通过，但性能显著低于正式版本：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.106778
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.007861

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.138847
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.014734

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.442651
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.061026
```

### 是否保留

不保留。

原因：

- 虽然不改变时间向量组成并保持 Correct
- 但每个 x 都需要 OpenMP 同步，屏障开销很大
- 独立 y vector chunk 取消了正式版本跨 chunk 复用 `r0..r4` 的热路径，重复加载增加
- 中等规模已比正式版本慢约 7 倍，没有必要继续跑大规模或替换正式文件

## 第六次迭代：y-block 独立 x 扫描并行化实验（不保留）

### 问题

第三次 y-block strip-mining 在 x-major 调度下 Correct，但性能低于正式版本。为了保留时间向量组成，同时尝试获得 y block 级并行性，实验把每个 y block 改成独立执行完整 x 扫描。

### 修改方法

新建实验入口 `blocking_parallel_rectangle_vectime_extra_array_exp`：

- 每个 y block 使用私有 `BV0..BV5` 状态
- 保持连续 y lane 与 `Input_Output_1..4` 组成不变
- 每个 block 只回写 owner 行，halo 只参与计算
- 用 `#pragma omp parallel for schedule(static)` 并行处理 y blocks

### 测试结果

小规模从 `T=4` 开始即 Wrong，且错误大量出现在 y=4 之后，不是单纯边界噪声：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.000161
```

中小规模继续 Wrong：

```text
64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.442811
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.000790

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.553217
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.478750
```

### 是否保留

不保留。

原因：

- 该实验不 Correct
- 把原来的 `x outer / yblock inner` 调度改成 `yblock outer / x inner` 并不等价
- 即使每个 y block 使用私有 BV 状态，完整 x 扫描也会让 halo 行按独立状态演化，破坏全局 x-sweep 的时间状态顺序
- 第三次实验的 Correctness 依赖原 x-major 顺序，不能直接改成 yblock 并行

## 第五次迭代：局部工作区 y-block 并行实验（不保留）

### 问题

第三、四次 strip-mining 版本 Correct 但性能不足。为了避免改动时间向量组成，尝试把每个 y block 拷贝到带 halo 的局部工作区，在局部工作区中直接运行已验证的 4 步 x-time 内核，再只回写 owner 行。这样理论上可以按 y block 并行，同时不改 `Input_Output_1..4` 和 `BV0..BV5` 的组成。

### 修改方法

新建实验入口 `blocking_parallel_rectangle_vectime_extra_array_exp`：

- 将当前正式 x-time 内核改名为实验文件内的 `shapiro_vectime_full_tile`
- 每个 y block 拷贝 owner + halo 到局部 `LB[2][NX+2*XSTART][local_NY+2*YSTART]`
- 初始时把两个时间层都设为当前全局 source parity
- 调用 `shapiro_vectime_full_tile(..., T=VECLEN, ...)`
- 只把 owner 行的 final parity 回写到全局数组

### 测试结果

`T=4` 小测试 Correct，但性能极低，拷贝/分配开销明显：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.000133
```

`T=8` 开始 Wrong：

```text
64 64 8 32 32 4
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp

126 151 166 96 64 12
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp
```

进一步把 halo 从 8 行增加到 16 行后，`64 64 8 32 32 4` 仍然 Wrong，说明错误不是简单 halo 深度不足，而是跨 4 步 tile 时局部工作区没有保留正式内核依赖的中间层/边界状态。

### 是否保留

不保留。

原因：

- `T>VECLEN` 不 Correct
- 即使 `T=VECLEN` Correct，性能也远低于正式版本
- 该方案虽然不改时间向量组成，但局部化破坏了跨 tile 状态假设

## 第四次迭代：strip-mining 热路径去分支与 halo 缩小验证（不保留）

### 问题

第三次 y-block strip-mining 虽然 Correct，但性能低于正式版本。需要确认主要开销是否来自热路径里的 `load_sum_block` 分支和 8 行 halo 重算。

### 修改方法

1. 在实验文件中，把主 y-chunk 热路径恢复为直接 `load_sum_from_bv`，只在低/高边界和 tail 使用 `load_sum_block`。
2. 继续验证更激进的 halo 缩小：把 `y_halo` 从 `YSLOPE * VECLEN = 8` 改成 `YSLOPE = 2`。

### 测试结果

热路径去分支后仍 Correct，但中小规模没有稳定超过正式版本：

```text
64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.252062

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.408685
```

随后把 halo 缩到 2 行后，Correctness 直接失败：

```text
31 31 4 24 24 4
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp

64 64 8 32 32 4
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp

126 151 166 96 64 12
Wrong! blocking_parallel_rectangle_vectime_extra_array_exp
```

错误集中在 y block 边界附近，说明 25p radius=2 在 4 步时间向量中仍需要 `YSLOPE * VECLEN` 的依赖闭包；只保留单步半径会破坏时间向量计算结果。

恢复 8 行 halo 并保留热路径直接 BV 读取后，大规模仍未超过正式版：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.155477
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.994155
```

### 是否保留

不保留为正式版本。

处理：

- `y_halo = YSLOPE` 已回退为 `y_halo = YSLOPE * VECLEN`
- 热路径直接 BV 读取只留在实验文件中继续观察
- 正式文件不替换

## 第三次迭代：y-block extra-array strip-mining 实验（不保留）

### 问题

正式 256 版本仍是全 y 范围的 x-time extra-array 内核，`xb/yb/tb` 只保持接口兼容，没有体现 y 方向分块。需要在不改变时间向量组成的前提下尝试分块。

### 修改方法

新建实验文件：

```text
blocking_parallel_rectangle_vectime_extra_array_exp.c
```

实验策略：

- 保持 `Input_Output_1..4`、连续 y lane、`BV0..BV5` 轮转语义不变
- 按 `yb` 把 y 方向切成 owner block
- 每个 block 额外保留 `YSLOPE * VECLEN = 8` 行 halo，避免内部块边界直接读取已经被其它 block 改写的全局 B
- 每个 block 用独立 `bv_head` 维护 6 个 BV slice 的轮转
- halo 只参与计算，最终只提交 owner 行，避免用边界 halo 的近似结果覆盖邻块正式输出

### 测试结果

Correctness 全部通过：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.085422

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.230761

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.545855

4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array_exp, GStencil/s = 0.869432
```

同一轮正式版本性能：

```text
126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.577600

4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.045639
```

### 是否保留

不保留为正式版本。

原因：

- 虽然没有破坏时间向量组成，且所有测试 Correct
- 但大规模性能比当前正式版本低约 16.9%
- 主要开销来自 block halo 重算、owner-store mask、`load_sum_block` 边界判断和每个 y block 的独立 BV 状态维护

该实验文件仅作为下一轮优化参考；在没有明显性能收益前不替换正式函数。

## 第二次迭代：保持连续 y lane 的右侧输入预取与边界整理

### 目标

在不改变当前 256 x-time extra-array 内核依赖结构的前提下，尝试低风险 cache 优化。

必须保持：

- `Input_Output_1..4` 不变
- `BV0..BV5` 指针轮转不变
- feature row window 和 y 跨 chunk row 复用逻辑不变
- lane 仍是连续 y：`y, y+1, y+2, y+3`
- 不做 true y-wavefront blocking，也不做 even/odd phase split

### 问题

当前稳定版在 x 主体中每个 y chunk 都会从普通 `B` 布局读取：

```c
B[t%2][x + STRIDE * VECLEN][y]
```

这个右侧输入行在下一次 x 迭代会变成新的右边界输入。由于 x 行跨度约为一整行 `NY + 2 * YSTART`，硬件顺序预取很难提前把下一 x 行带入 cache。

另一个小问题是热路径中多次重复计算完整 y 向量区间边界，虽然编译器可能会消掉一部分，但代码可读性和边界一致性较差。

### 修改方法

先新建过实验文件：

```text
blocking_parallel_rectangle_vectime_extra_array_exp.c
```

实验内容：

1. 在函数入口 hoist y 边界：

```c
const int y_vec_last = YSTART + ((NY - VECLEN) / VECLEN) * VECLEN;
const int y_vec_limit = y_vec_last + VECLEN - 1;
const int y_tail_begin = y_vec_last + VECLEN;
```

2. 在低边界、主路径、高边界三个 y 向量循环中，对下一 x 迭代将使用的右侧输入行做预取：

```c
_mm_prefetch((const char *)
    &B[t%2][x + STRIDE * VECLEN + 1][y], _MM_HINT_T0);
```

3. 确认实验版 Correct 后，把同一组修改合入正式文件：

```text
blocking_parallel_rectangle_vectime_extra_array.c
```

随后删除实验文件和临时 `main.c`/`define.h` 接入口，最终默认构建仍只测试正式函数。

### 测试结果

基础正确性测试全部通过：

```text
31 31 4 24 24 4
64 64 8 32 32 4
126 151 166 96 64 12
```

最终正式文件测试结果：

```text
31 31 4 24 24 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.073923

64 64 8 32 32 4
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.455111

126 151 166 96 64 12
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.576861
```

大规模 baseline：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 0.480853
```

实验版曾在同一进程中紧跟稳定版运行，出现过 `1.175218` 和 `1.179085 GStencil/s`。这组数值受函数运行顺序和热缓存影响，不能直接作为最终收益。

替换正式文件后，在默认测试顺序中重复大规模：

```text
4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.049298

4125 2125 1162 1536 1024 256
Correct! blocking_parallel_rectangle_vectime_extra_array, GStencil/s = 1.074212
```

相对本轮 baseline `0.480853 GStencil/s`，正式位置仍有明确收益。

### 是否保留

保留。

原因：

- 没有改变 temporal-vector 依赖和 lane 布局
- 所有基础测试和大规模测试均 Correct
- 大规模正式测试顺序下稳定优于当前 baseline
- 修改局部，风险主要是 `_mm_prefetch` hint 对不同机器收益可能变化，但不会改变计算结果

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
