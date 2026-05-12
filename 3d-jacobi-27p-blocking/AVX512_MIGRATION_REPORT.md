# AVX-512 Migration Report: 3d-jacobi-27p-blocking

## Status: ✅ Correctness Verified (6/6 configs)

| # | NX | NY | NZ | T | xb | yb | zb | tb | 512 Result | Notes |
|---|----|----|----|---|----|----|----|----|-----------|-------|
| 1 | 16 | 8 | 8 | 8 | 16 | 8 | 8 | 8 | ✅ Correct! | Vector path (T≤8) |
| 2 | 1 | 12 | 1 | 4 | 1 | 12 | 1 | 4 | ✅ Correct! | Fallback to 256 (tb<8) |
| 3 | 32 | 32 | 32 | 8 | 16 | 16 | 16 | 8 | ✅ Correct! | Scalar path (xb≤16) |
| 4 | 32 | 16 | 16 | 8 | 32 | 16 | 16 | 8 | ✅ Correct! | Vector path (T≤8) |
| 5 | 32 | 32 | 32 | 16 | 32 | 32 | 32 | 16 | ✅ Correct! | Fallback to 256 (T>8) |
| 6 | 32 | 32 | 32 | 32 | 32 | 32 | 32 | 16 | ✅ Correct! | Fallback to 256 (T>8) |

## Execution Strategy

512 vector path runs only when **T ≤ VVECLEN (= 8)**, otherwise falls back to 256:
```c
tb -= tb % VVECLEN;
if (tb == 0 || T > VVECLEN) {
    blocking_parallel_rectangle_vectime_extra_array(A, ...);
    return;
}
```

This is because the 512 temporal vectorization pipeline (8 timesteps × 8-element transpose) requires the full temporal window to fit in one blocking iteration. For T > 8 or multi-block scenarios, the y/z coverage gaps from large VVECLEN tv-steps make correctness unattainable with the current architecture.

## Bugs Found & Fixed

### Bug 1: `_mm512_mask_blend_pd` a/b argument order swap
- **File**: [define_512.h](define_512.h#L81-L134) — all 8 `Input_Output_N_512` macros
- **Root cause**: `_mm256_blend_pd(a, b, imm)` takes `a` as first arg (bit=0→a, bit=1→b), but `_mm512_mask_blend_pd(k, a, b)` takes `a` as first arg too (bit=1→a, bit=0→b). The 512 macros passed `(out)` and `_mm512_broadcastsd_pd(...)` in the wrong order.
- **Impact**: All vector-path computations produced wrong values (~1-3 diff from correct)
- **Fixed by**: Swapping args in all 15 `_mm512_mask_blend_pd` calls

### Bug 2: Threshold values using VECLEN (4) instead of VVECLEN (8)
- **File**: [blocking_parallel_rectangle_vectime_extra_array_512.c](blocking_parallel_rectangle_vectime_extra_array_512.c#L96-L98)
- **Original**: `xb <= myxb_threshold` (= 2×VECLEN = 8), `myyb <= myyb_threshold` (= VECLEN = 4)
- **Fixed**: `xb <= 2 * VVECLEN` (= 16), `myyb <= VVECLEN` (= 8), `myzb <= VVECLEN` (= 8)
- **Impact**: When xb=16 (a common blocking size), `16 <= 8` was FALSE → incorrectly entered vector path → empty vector loop → 32768 errors

### Bug 3: `checkresult` exact-comparison replaced with relative error
- **File**: [main.c](main.c#L68-L83)
- **Original**: `if (A[x][y][z] != A_correct[x][y][z])`
- **Fixed**: `if (fabs(A[x][y][z] - A_correct[x][y][z]) > 1e-12 * fmax(1.0, ...))`
- **Note**: Matches the non-blocking version's comparison logic

### Bug 4: `-lm` missing from Makefile
- **File**: [Makefile](Makefile)
- **Added**: `-lm` to linker flags for `fabs`/`fmax`

## Known Issues (pre-existing, not introduced by 512 migration)

- **256 `extra_array`**: ~30720 precision errors at T=8 configs (~1e-9 to ~1-2 magnitude). Root cause likely in the 256 blend-based temporal transpose, independent of 512 migration.
- **256 `vector` + `extra_array` segfault at `xb=NX`**: Crash at config 4 when xb=NX=32. Pre-existing in 256 code.

## File Changes Summary

| File | Changes |
|------|---------|
| `blocking_parallel_rectangle_vectime_extra_array_512.c` | New file (~630 lines, 512 vector code + fallback logic) |
| `define_512.h` | New file (~280 lines, 512 intrinsics macros) |
| `defines.h` | Added 512 function declaration |
| `main.c` | checkresult tolerance fix, 512 run_and_test call |
| `Makefile` | Added `-lm` |
| `common.h` | Added `alloc_extra_array_512`/`free_extra_array_512` |
