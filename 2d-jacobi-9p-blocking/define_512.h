#ifndef DEFINE_512_H
#define DEFINE_512_H

// #define DEBUG_512
// #define DEBUG_512_TRACE

#include "define.h"

//强制开启512位，否则error
#ifndef __AVX512F__
#error "define_512.h requires AVX-512F. Compile with -mavx512f."
#endif

// ============================================================
// AVX-512 blocking 专用宏
// 策略：所有宏/类型带 _512 后缀或使用 __m512d 直接声明
// 不覆盖 define.h 中的任何定义
// ============================================================

#define VVECLEN 8

// AVX-512 加载存储宏 (blocking版本，参数带取地址)
#define vload_512_blk(a,b) (a) = _mm512_loadu_pd(&(b))
#define vstore_512_blk(a,b) _mm512_storeu_pd(&(a),(b))
#define vloada_512(a,b) (a) = _mm512_load_pd(&(b))
#define vstorea_512(a,b) _mm512_store_pd(&(a),(b))

#define loadv_512_blk(b) _mm512_loadu_pd(&(b))
#define storev_512_blk(a,b) _mm512_storeu_pd(&(a),(b))

#define alloc_extra_array_512(size) ({  \
        void *ptr = NULL;                       \
        if (posix_memalign(&ptr, 64, size) != 0) { ptr = NULL; } \
        ptr;                                \
})

// 旋转宏
#define vrotate_512_low2high(a) \
    _mm512_castsi512_pd(_mm512_alignr_epi64(_mm512_castpd_si512(a), _mm512_castpd_si512(a), 1))

#define vrotate_512_high2low(a) \
    _mm512_castsi512_pd(_mm512_alignr_epi64(_mm512_castpd_si512(a), _mm512_castpd_si512(a), 7))

// 8x8 转置宏
#define transpose8x8_pd_512(r0,r1,r2,r3,r4,r5,r6,r7) do {                      \
    __m512d t0  = _mm512_unpacklo_pd((r0),(r1));                                \
    __m512d t1  = _mm512_unpackhi_pd((r0),(r1));                                \
    __m512d t2  = _mm512_unpacklo_pd((r2),(r3));                                \
    __m512d t3  = _mm512_unpackhi_pd((r2),(r3));                                \
    __m512d t4  = _mm512_unpacklo_pd((r4),(r5));                                \
    __m512d t5  = _mm512_unpackhi_pd((r4),(r5));                                \
    __m512d t6  = _mm512_unpacklo_pd((r6),(r7));                                \
    __m512d t7  = _mm512_unpackhi_pd((r6),(r7));                                \
                                                                                \
    const __m512i idx0 = _mm512_setr_epi64(0, 1, 8, 9, 4, 5, 12, 13);           \
    const __m512i idx1 = _mm512_setr_epi64(2, 3,10,11, 6, 7, 14, 15);           \
                                                                                \
    __m512d tt0 = _mm512_permutex2var_pd(t0, idx0, t2);                         \
    __m512d tt1 = _mm512_permutex2var_pd(t0, idx1, t2);                         \
    __m512d tt2 = _mm512_permutex2var_pd(t1, idx0, t3);                         \
    __m512d tt3 = _mm512_permutex2var_pd(t1, idx1, t3);                         \
    __m512d tt4 = _mm512_permutex2var_pd(t4, idx0, t6);                         \
    __m512d tt5 = _mm512_permutex2var_pd(t4, idx1, t6);                         \
    __m512d tt6 = _mm512_permutex2var_pd(t5, idx0, t7);                         \
    __m512d tt7 = _mm512_permutex2var_pd(t5, idx1, t7);                         \
                                                                                \
    (r0) = _mm512_shuffle_f64x2(tt0, tt4, 0x44);                               \
    (r1) = _mm512_shuffle_f64x2(tt2, tt6, 0x44);                               \
    (r2) = _mm512_shuffle_f64x2(tt1, tt5, 0x44);                               \
    (r3) = _mm512_shuffle_f64x2(tt3, tt7, 0x44);                               \
    (r4) = _mm512_shuffle_f64x2(tt0, tt4, 0xEE);                               \
    (r5) = _mm512_shuffle_f64x2(tt2, tt6, 0xEE);                               \
    (r6) = _mm512_shuffle_f64x2(tt1, tt5, 0xEE);                               \
    (r7) = _mm512_shuffle_f64x2(tt3, tt7, 0xEE);                               \
} while(0)

#define transpose_512(a,b,c,d,e,f,g,h,i,j) do { \
    transpose8x8_pd_512(a,b,c,d,e,f,g,h); \
    i = a; j = b; \
} while(0)

// ============================================================
// 计算宏 (AVX-512 版本，带 _512 后缀)
// 9点模板: 中心 + 8邻域(上下左右+4对角线)
// ============================================================

#ifdef simplestencil

#define C0_512 0.111111111111
#define SET_COFF_512  __m512d vc0_512 = _mm512_set1_pd(C0_512)

// 9点模板：9个输入参数，对应3行x3列
// v_x_minus_2, v_center_2, v_x_plus_2 : 第y-1行 (上一行)
// v_x_minus_1, v_center_1, v_x_plus_1 : 第y行   (当前行)
// v_x_minus_0, v_center_0, v_x_plus_0 : 第y+1行 (下一行)
#define Compute_1vector_512(v_x_minus_2, v_center_2, v_x_plus_2, \
                            v_x_minus_1, v_center_1, v_x_plus_1, \
                            v_x_minus_0, v_center_0, v_x_plus_0) \
    v_center_0 = _mm512_mul_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd( \
        v_x_minus_0, v_center_0), v_x_plus_0), \
        v_x_minus_1), v_center_1), v_x_plus_1), \
        v_x_minus_2), v_center_2), v_x_plus_2), vc0_512)

#else

#define C0_512 0.12
#define C1_512 0.11
#define SET_COFF_512  __m512d vc0_512 = _mm512_set1_pd(C0_512); __m512d vc1_512 = _mm512_set1_pd(C1_512)

// 9点模板：C0 * center + C1 * (其他8点之和)
#define Compute_1vector_512(v_x_minus_2, v_center_2, v_x_plus_2, \
                            v_x_minus_1, v_center_1, v_x_plus_1, \
                            v_x_minus_0, v_center_0, v_x_plus_0) \
    v_center_0 = _mm512_add_pd(_mm512_mul_pd(v_center_1, vc0_512), \
        _mm512_mul_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd(_mm512_add_pd( \
            v_x_minus_0, v_center_0), v_x_plus_0), \
            v_x_minus_1), v_x_plus_1), \
            v_x_minus_2), v_center_2), v_x_plus_2), vc1_512))

#endif

// Output/Input_Output 宏 (AVX-512 版本，带 _512 后缀)
#define Output_1_512(out, v1) out = v1
#define Output_2_512(out, v1) out = _mm512_mask_blend_pd(0x02, (out), (v1))
#define Output_3_512(out, v1) out = _mm512_mask_blend_pd(0x04, (out), (v1))
#define Output_4_512(out, v1) out = _mm512_mask_blend_pd(0x08, (out), (v1))
#define Output_5_512(out, v1) out = _mm512_mask_blend_pd(0x10, (out), (v1))
#define Output_6_512(out, v1) out = _mm512_mask_blend_pd(0x20, (out), (v1))
#define Output_7_512(out, v1) out = _mm512_mask_blend_pd(0x40, (out), (v1))
#define Output_8_512(out, v1) out = _mm512_mask_blend_pd(0x80, (out), (v1))

#define Input_Output_1_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = (v1); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_2_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x02, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_3_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x04, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_4_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x08, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_5_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x10, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_6_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x20, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_7_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x40, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
    (in) = vrotate_512_low2high(in); \
} while(0)

#define Input_Output_8_512(out, v1, in) do { \
    (v1) = vrotate_512_high2low(v1); \
    (out) = _mm512_mask_blend_pd(0x80, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1))); \
    (v1) = _mm512_mask_blend_pd(0x01, (v1), (in)); \
} while(0)

// ============================================================
// AVX-512 BV Buffer 访问宏 (blocking 版本)
// 对应 AVX-256 中的 loadv_2d_x_m_blk / loadv_2d_x_c_blk / loadv_2d_x_p_blk / vstore_2d_x_pp_blk
// 使用 VVECLEN=8 代替 VECLEN=4
// ============================================================

#define loadv_512_2d_x_m_blk(y) 	_mm512_loadu_pd(&BV0[ y - (myybeg - VVECLEN + 1) ][ 0 ])
#define loadv_512_2d_x_c_blk(y) 	_mm512_loadu_pd(&BV1[ y - (myybeg - VVECLEN + 1) ][ 0 ])
#define loadv_512_2d_x_p_blk(y) 	_mm512_loadu_pd(&BV2[ y - (myybeg - VVECLEN + 1) ][ 0 ])
#define vstore_512_2d_x_pp_blk(v, y) _mm512_storeu_pd(&BV3[ y - (myybeg - VVECLEN + 1) ][ 0 ], v)

// ============================================================
// Blocking 专用宏 (AVX-512 版本，带 _512 后缀)
// ============================================================

// 9p 模板需要3行数据，使用3行buffer: BV0(上一行), BV1(当前行), BV2(下一行)
// 输出到 BV3
#define Compute_one_512(v_center_3, v_center_0, v_center_1, v_center_2, a, b) \
    v_center_2 = loadv_512_2d_x_c_blk(y + a); \
    vstore_512_2d_x_pp_blk(v_center_3, y + a - 1); \
    Compute_1vector_512(v_center_0, v_center_1, v_center_2, \
                    loadv_512_2d_x_m_blk(y + a - 1), \
                    loadv_512_2d_x_p_blk(y + a - 1)); \
    Input_Output_##b##_512(out, v_center_0, in)

#define Compute_lastone_512(v_center_2, v_center_3, v_center_0, v_center_1, a, b) \
    v_center_1 = ( y > myybeg - VVECLEN + 1 + myyb - VVECLEN - VVECLEN * 2 ) ? \
                    setv_512_2d_blk(x, y + a) \
                 :  loadv_512_2d_x_c_blk(y + a) ; \
    v_x_minus = loadv_512_2d_x_m_blk(y + a - 1); \
    vstore_512_2d_x_pp_blk(v_center_2, y + a - 1); \
    Compute_1vector_512(v_center_3, v_center_0, v_center_1, \
                    v_x_minus, \
                    loadv_512_2d_x_p_blk(y + a - 1)); \
    Input_Output_##b##_512(out, v_center_3, in)

#define setv_512_2d_blk(x, y) _mm512_set_pd( \
    B[(t+1)%2][x+STRIDE*0][y+0], B[(t)%2][x+STRIDE*1][y+1], B[(t+1)%2][x+STRIDE*2][y+2], B[(t)%2][x+STRIDE*3][y+3], \
    B[(t+1)%2][x+STRIDE*4][y+4], B[(t)%2][x+STRIDE*5][y+5], B[(t+1)%2][x+STRIDE*6][y+6], B[(t)%2][x+STRIDE*7][y+7])

#define vstore_set_512_2d_blk(x, y, v) vstore_512_blk(tmp[0], v); \
    B[(t+1)%2][x+STRIDE*7][y+7]=tmp[0]; B[t%2][x+STRIDE*6][y+6]=tmp[1]; \
    B[(t+1)%2][x+STRIDE*5][y+5]=tmp[2]; B[t%2][x+STRIDE*4][y+4]=tmp[3]; \
    B[(t+1)%2][x+STRIDE*3][y+3]=tmp[4]; B[t%2][x+STRIDE*2][y+2]=tmp[5]; \
    B[(t+1)%2][x+STRIDE*1][y+1]=tmp[6]; B[t%2][x+STRIDE*0][y+0]=tmp[7]

#define vstore_set_512_2d_x_m_blk(x, y, v) vstore_512_blk(tmp[0], v); \
    B[(t)%2][x+STRIDE*7][y+7]=tmp[0]; B[(t+1)%2][x+STRIDE*6][y+6]=tmp[1]; \
    B[(t)%2][x+STRIDE*5][y+5]=tmp[2]; B[(t+1)%2][x+STRIDE*4][y+4]=tmp[3]; \
    B[(t)%2][x+STRIDE*3][y+3]=tmp[4]; B[(t+1)%2][x+STRIDE*2][y+2]=tmp[5]; \
    B[(t)%2][x+STRIDE*1][y+1]=tmp[6]; B[(t+1)%2][x+STRIDE*0][y+0]=tmp[7]

// 函数声明
void blocking_parallel_rectangle_vectime_512(double *A, int NX, int NY, int T, int xb, int yb, int tb);
void blocking_parallel_rectangle_vectime_extra_array_512(double *A, int NX, int NY, int T, int xb, int yb, int tb);
void blocking_parallel_rectangle_vectime_512_hybrid(double *A, int NX, int NY, int T, int xb, int yb, int tb);
void blocking_parallel_rectangle_vectime_extra_array_512_hybrid(double *A, int NX, int NY, int T, int xb, int yb, int tb);

#endif /* DEFINE_512_H */
