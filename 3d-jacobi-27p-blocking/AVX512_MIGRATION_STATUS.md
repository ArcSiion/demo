# 3D Jacobi 27p Blocking AVX-512 迁移进度报告

> 分支: `migrate/3d-jacobi-27p-blocking-avx512`
> 日期: 2026-04-29

---

## 一、已完成工作

### 1. 清理旧代码
- 删除了 `3d-jacobi-27p-blocking/` 中已有的 `blocking_parallel_rectangle_vectime_extra_array_512.c` 和 `define_512.h`

### 2. 参考模板分析
- 完整阅读 `3d-jacobi-7p-blocking/` 中的 AVX-512 实现（`define_512.h`、`blocking_parallel_rectangle_vectime_extra_array_512.c`）
- 识别关键模式：8-step LRU、8x8 transpose、Input_Output_K_512 宏、boundary writeback 序列

### 3. 重写 AVX-512 实现

#### define_512.h (262 行)
- Compute_1vector_512 宏（27 邻居版本，10 参数）
- Input_Output_1..8_512 宏（与 7p 模板完全一致）
- transpose_512 宏（8x8 transpose，与 7p 模板一致）
- setv_3d_blk_512、vstore_set_3d_blk_512 宏
- load_v_512、store_v_512 宏

#### blocking_parallel_rectangle_vectime_extra_array_512.c (570 行)
- HEAD transpose (B → Btmp)
- MAIN 主循环 8-step 实现：
  - v_center 8 寄存器滑窗 LRU
  - d_1/d_2 三槽位 LRU (mod 3 与 mod 8 不对齐)
  - 末尾旋转 `temp=_0; _0=_2; _1=temp`
  - 8-step boundary writeback (K=7..0)
- Tail head transpose (B → BV3, y=first row)
- Tail transpose (BV0 → B, y=last row)
- Tail transpose (Btmp → B, x=xmax-STRIDE*VVECLEN..+2)

### 4. 集成与编译
- main.c 添加 _512 测试调用
- Makefile 添加 _512.c 编译项
- 远端 ict 服务器编译通过（gcc -O3 -mavx512f）

---

## 二、测试结果

### 测试环境
- 服务器：ict (122.9.36.35)
- 参数：`NX=128 NY=128 NZ=128 T=100 xb=32 yb=32 zb=32 tb=16`
- 线程：`OMP_NUM_THREADS=1`（正确性验证阶段）

### 测试 1：仅 _256（注释掉 _512）
| 实现 | 结果 | 备注 |
|------|------|------|
| `blocking_parallel_rectangle_scalar` | ✅ Correct | 基准参考 |
| `blocking_parallel_rectangle_vector` | ✅ Correct | 向量版参考 |
| `blocking_parallel_rectangle_vectime_extra_array` (AVX-256) | ❌ 错误 | 1e-9 量级浮点误差 |

**关键发现**：_256 实现与 scalar 的差异在 **1e-9 量级**（如 `541.089646488874450` vs `541.089646489676284`，差异约 8×10⁻¹⁰），这是 **浮点累加顺序差异**，超出 main.c 的 1e-12 容差。这是 **_256 实现的历史遗留问题**，非 _512 引入。

### 测试 2：_256 vs _512 直接对比
修改 main.c 让 A_correct 持有 _256 结果，与 _512 直接对比：
- **错误数量**：2,097,152 个（恰好 = 128³，整个内部域全错）
- **差异量级**：0.04 ~ 0.25（远超浮点精度）
- **结论**：_512 实现存在 **结构性 bug**，与 _256 完全不一致

---

## 三、已排查的正确部分

通过详细 LRU 分析和与 7p _512 模板逐行对比，确认以下部分**正确**：

1. ✅ **v_center 8 寄存器滑窗**：每步 target = v_center_(k-1)%8，加载 (k+1)%8，dummy = (k-2)%8
2. ✅ **d_1/d_2 三槽位 LRU**：每步加载 slot (k+1)%3
3. ✅ **末尾 d_1/d_2 旋转**：`temp=_0; _0=_2; _1=temp`
4. ✅ **Input_Output_K_512 宏**：与 7p _512 完全一致
5. ✅ **transpose_512 宏**：与 7p _512 完全一致
6. ✅ **setv_3d_blk_512 宏**：与 7p _512 完全一致
7. ✅ **vstore_set_3d_blk_512 宏**
8. ✅ **boundary writeback 索引**：BV0[y_idx][myzb-1][0..7]，与 7p _512 / 27p _256 一致
9. ✅ **carry 语义**：v_center_7 三阶段 (Step 1 carry input → Step 6 被覆盖 → Step 8 写为 output)

---

## 四、待排查的潜在 bug 位置

以下部分尚未完成与 _256 的逐行对比：

1. **HEAD transpose (B → Btmp)**
   - 8 个 vload_512_blk 的 STRIDE 索引（*7..0）
   - t parity 序列（(t)%2, (t+1)%2 交替）
   - 与 _256 的 4-step 扩展为 8-step 是否一致

2. **tail head transpose (B → BV3, y=first row)**
   - vstore 顺序与 lane 映射
   - BV3 写入索引

3. **tail transpose (BV0 → B, y=last row)**
   - lane 0..7 → vstore 顺序
   - 与 7p _512 / 27p _256 是否完全等价

4. **Tail transpose (Btmp → B, x=xmax-STRIDE*VVECLEN..+2)**
   - x 索引 `x - (xmax - STRIDE * VVECLEN)` 偏移计算
   - t parity 序列

5. **Step 8 carry 处理与下一轮 store_x_pp(v_center_7) 时序**

---

## 五、下一步计划

1. **系统性对比四个 transpose 段**：
   - _256 vs _512 逐行对比
   - 重点检查 STRIDE 索引、t parity、xshift/yshift/zshift
2. **定位并修复 bug**：
   - 优先检查 HEAD transpose 的索引和 parity
   - 再检查 tail head / tail transpose 的 lane 映射
3. **重新编译测试**：
   - _512 vs _256 直接对比
   - 目标：0 错误（或仅在浮点精度容差内）
4. **性能测试**（通过正确性后）：
   - 32 线程，大规模参数
   - 对比 AVX-256 / AVX-512 性能

---

## 六、技术要点总结

### 27p vs 7p 关键差异
| 特性 | 7p | 27p |
|------|-----|-----|
| 邻居数 | 7（中心 + 6 面） | 27（中心 + 6 面 + 12 边 + 8 角） |
| Compute_1vector 参数 | 7 | 10（第 4 参数为 dummy） |
| d_1/d_2 计算 | 单个加载 | 多向量加法（Add_4_vectors_512） |

### AVX-256 → AVX-512 迁移规则
- VECLEN=4 → VVECLEN=8
- __m256d → __m512d
- 4-step → 8-step
- 4x4 transpose → 8x8 transpose
- 4 个 v_center → 8 个 v_center
- d_1/d_2 末尾旋转：2 步 → 3 步 (mod 3)

### LRU 模式（8-step）
- **v_center**: mod 8 循环，步 k 加载 (k+1)%8
- **d_1/d_2**: mod 3 循环，步 k 加载 (k+1)%3
- **末尾旋转**: `temp=_0; _0=_2; _1=temp`（重排为下一轮起始态）
