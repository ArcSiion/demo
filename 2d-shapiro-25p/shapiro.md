# 2D Shapiro 25-point Stencil

`2d-shapiro-25p` 实现的是一个 **radius = 2 的 2D Shapiro 平滑滤波 stencil**。它的计算区域是 5x5，因此每次更新读取 25 个点；但它不是普通的 25 点平均滤波，而是由一维四阶 Shapiro filter 在 x/y 两个方向做 tensor product 得到的可解释 stencil。

这个目录存在的意义是：给 TemporalStencil 增加一个来自真实数值模式调研的宽 stencil benchmark。它比常见的 `2d-jacobi-5p` / `2d-jacobi-9p` 更宽，系数中包含负值，并且仍然保持 Jacobi-style 双数组时间推进，适合后续接入 temporal vectorization。

## 来源与动机

Shapiro filter 最早由 Ralph Shapiro 系统讨论，用于有限差分数值积分中的 smoothing/filtering。它的目标不是简单扩散所有尺度，而是选择性地抑制网格尺度高频噪声，同时尽量保留低频、长波结构。

这类滤波在真实数值模式中也有使用。例如 MITgcm 文档将 Shapiro filter 描述为 high order horizontal filter，用于在时间步结束时作用于 velocity 和 tracer fields，以移除小尺度网格噪声且尽量不破坏物理结构。MITgcm 还提供了独立的 `pkg/shap_filt/` 包。

因此，本项目不是为了凑一个 25 点 kernel，而是把一个有实际数值意义的 filter 写成 TemporalStencil 可运行、可验证、可继续优化的 stencil benchmark。

![alt text](image.png)

## 当前采用的滤波形式

本项目采用的是 radius = 2 的一维四阶 Shapiro filter，然后把它扩展到二维。

一维情况下，新值由当前位置和左右各两个点组成：

| offset | `-2` | `-1` | `0` | `+1` | `+2` |
|---|---:|---:|---:|---:|---:|
| weight | `W2` | `W1` | `W0` | `W1` | `W2` |

也就是：

```text
new[i] = W2 * old[i-2]
       + W1 * old[i-1]
       + W0 * old[i]
       + W1 * old[i+1]
       + W2 * old[i+2]
```

代码中的权重定义在 `define.h`：

```c
#define SHAP_MU 1.0

#define W0 (1.0 - 3.0 * SHAP_MU / 8.0)
#define W1 (SHAP_MU / 4.0)
#define W2 (-SHAP_MU / 16.0)
```

当 `SHAP_MU = 1.0` 时：

| weight | value | decimal |
|---|---:|---:|
| `W0` | `5/8` | `0.625` |
| `W1` | `1/4` | `0.25` |
| `W2` | `-1/16` | `-0.0625` |

这组系数有两个重要性质：

- 常数场保持不变：`W0 + 2*W1 + 2*W2 = 1`。
- 高频被选择性抑制：它对应四阶 Shapiro filtering，对 2-grid-interval wave 有很强抑制；在 `SHAP_MU = 1.0` 时，最短波长的交替振荡会被消掉。

## 二维 25 点系数

二维版本采用 separable 形式：x 方向用一次一维 Shapiro filter，y 方向也用同样的 filter。等价地，二维 5x5 权重矩阵就是一维权重向量 `[W2, W1, W0, W1, W2]` 和自身的外积。

按 `dy` 行、`dx` 列写成表格：

| `dy \ dx` | `-2` | `-1` | `0` | `+1` | `+2` |
|---|---:|---:|---:|---:|---:|
| `-2` | `C22` | `C12` | `C02` | `C12` | `C22` |
| `-1` | `C12` | `C11` | `C01` | `C11` | `C12` |
| `0`  | `C02` | `C01` | `C00` | `C01` | `C02` |
| `+1` | `C12` | `C11` | `C01` | `C11` | `C12` |
| `+2` | `C22` | `C12` | `C02` | `C12` | `C22` |

其中：

| coefficient | definition | when `SHAP_MU = 1.0` |
|---|---|---:|
| `C00` | `W0 * W0` | `25/64 = 0.390625` |
| `C01` | `W0 * W1` | `5/32 = 0.15625` |
| `C02` | `W0 * W2` | `-5/128 = -0.0390625` |
| `C11` | `W1 * W1` | `1/16 = 0.0625` |
| `C12` | `W1 * W2` | `-1/64 = -0.015625` |
| `C22` | `W2 * W2` | `1/256 = 0.00390625` |

`Compute_scalar` 正是按这个表展开的：

- `C00`：中心点 `(x, y)`。
- `C01`：四个一格轴向邻居 `(x+-1, y)` 和 `(x, y+-1)`。
- `C02`：四个两格轴向邻居 `(x+-2, y)` 和 `(x, y+-2)`。
- `C11`：四个一格对角邻居 `(x+-1, y+-1)`。
- `C22`：四个两格对角邻居 `(x+-2, y+-2)`。
- `C12`：八个混合偏移邻居 `(x+-1, y+-2)` 和 `(x+-2, y+-1)`。

这样分组后，公式不需要在文档中反复展开 25 项；读代码时只要对照上面的 5x5 表即可。

## 与 TemporalStencil 代码的对应

核心配置在 `define.h`：

```c
#define RADIUS 2

#define XSTART RADIUS
#define YSTART RADIUS
#define XSLOPE RADIUS
#define YSLOPE RADIUS
```

这些宏的含义：

| macro | meaning |
|---|---|
| `RADIUS` | stencil 的数学半径。Shapiro 25p 会读到 `x/y` 方向 `+-2`，因此取 2。 |
| `XSTART`, `YSTART` | interior 区域的起点，也就是 halo 宽度。 |
| `XSLOPE`, `YSLOPE` | temporal vectorization/blocking 中每推进一个时间步的依赖扩张宽度。当前先与 `RADIUS` 保持一致。 |
| `STRIDE` | 后续 256-bit temporal kernel 的实现/调度参数，不改变 Shapiro filter 的数学定义。 |

`naive_scalar.c` 是当前 reference kernel。它使用和 `2d-jacobi-9p` 一致的双数组 Jacobi-style 更新：

```text
A[(t + 1) % 2][x][y] = F(A[t % 2], x, y)
```

也就是说，每个时间步只读旧时间层，只写新时间层。虽然这个 stencil 的数值含义是滤波，不是热方程迭代，但它在数据依赖和性能建模上仍然是标准的 radius-2 2D Jacobi stencil。

## 为什么它适合作为 benchmark

`2d-shapiro-25p` 对项目有几个具体价值：

- 它是 5x5 box footprint，比 5p/9p 更能测试宽 stencil 的数据复用能力。
- 它来自 Shapiro filtering，而不是人为凑出的加权平均。
- 系数包含负值，更接近高阶滤波/高阶耗散算子。
- 它仍然是 Jacobi-style 双数组更新，方便复用现有 main/check/result 框架。
- 它适合作为后续 256-bit temporal vectorization 的扩展目标，因为 `RADIUS=2` 会显著改变 halo、slope 和边界处理压力。

## 当前实现状态

目前目录中的核心文件：

- `define.h`：定义半径、halo/slope、Shapiro 权重和 `Compute_scalar`。
- `naive_scalar.c`：scalar reference kernel。
- `main.c`：初始化、运行 reference、`checkresult` 和性能输出。
- `Makefile`：沿用项目公共编译配置。
- `test.sh`：给出一组运行参数。

`checkresult` 当前使用精确比较 `!=`，这是为了先对齐已有项目习惯。后续加入 SIMD/FMA 或其它计算重排后，浮点舍入路径可能不同，建议再切换到容差比较。

## 参考资料

1. Ralph Shapiro, "Smoothing, filtering, and boundary effects", Reviews of Geophysics, 1970. DOI: <https://doi.org/10.1029/RG008i002p00359>. CiNii 条目：<https://cir.nii.ac.jp/crid/1363388843561991552>
2. MITgcm User Manual, Section 2.18 "Shapiro Filter": <https://mitgcm.readthedocs.io/en/latest/algorithm/algorithm.html#shapiro-filter>
3. MITgcm `pkg/shap_filt/` package documentation: <https://mitgcm.readthedocs.io/en/latest/phys_pkgs/shap_filt.html>
4. F. Falissard, "Genuinely multi-dimensional explicit and implicit generalized Shapiro filters for weather forecasting, computational fluid dynamics and aeroacoustics", Journal of Computational Physics, 2013. DOI: <https://doi.org/10.1016/j.jcp.2013.07.001>
