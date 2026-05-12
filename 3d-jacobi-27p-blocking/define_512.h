#ifndef DEFINE_512_H
#define DEFINE_512_H

// 强制开启 512 位，否则报错
#ifndef __AVX512F__
#error "define_512.h requires AVX-512F. Compile with -mavx512f."
#endif

// ============================================================
// AVX-512 blocking 专用宏 (3d-jacobi-27p-blocking)
// 策略：所有宏/类型带 _512 后缀或使用 __m512d 直接声明
// 不覆盖 defines.h 中的任何 AVX-256 定义
// VECLEN = 4 (AVX-256), VVECLEN = 8 (AVX-512)
// ============================================================

// AVX-512 加载/存储宏 (blocking 版本，参数带取地址)
#define vload_512_blk(a,b)  (a) = _mm512_loadu_pd(&(b))
#define vstore_512_blk(a,b) _mm512_storeu_pd(&(a),(b))
#define vloada_512(a,b)     (a) = _mm512_load_pd(&(b))
#define vstorea_512(a,b)    _mm512_store_pd(&(a),(b))

#define loadv_512_blk(b)     _mm512_loadu_pd(&(b))
#define storev_512_blk(a,b)  _mm512_storeu_pd(&(a),(b))

#define vallset_512(a,b) { a = _mm512_set1_pd(b); }

#define myxb_threshold_512  2*VVECLEN
#define myyb_threshold_512  VVECLEN
#define myzb_threshold_512  VVECLEN

// 分配 64 字节对齐的额外数组
#define alloc_extra_array_512(size) ({  \
        void *ptr = NULL;                       \
        if (posix_memalign(&ptr, 64, size) != 0) { ptr = NULL; } \
        ptr;                                \
})

// 8 lane 旋转宏
#define vrotate_512_low2high(a) \
    _mm512_castsi512_pd(_mm512_alignr_epi64(_mm512_castpd_si512(a), _mm512_castpd_si512(a), 1))

#define vrotate_512_high2low(a) \
    _mm512_castsi512_pd(_mm512_alignr_epi64(_mm512_castpd_si512(a), _mm512_castpd_si512(a), 7))

// 8x8 转置宏
#define transpose8x8_pd_512(r0,r1,r2,r3,r4,r5,r6,r7) do {                       \
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
    (r0) = _mm512_shuffle_f64x2(tt0, tt4, 0x44);                                \
    (r1) = _mm512_shuffle_f64x2(tt2, tt6, 0x44);                                \
    (r2) = _mm512_shuffle_f64x2(tt1, tt5, 0x44);                                \
    (r3) = _mm512_shuffle_f64x2(tt3, tt7, 0x44);                                \
    (r4) = _mm512_shuffle_f64x2(tt0, tt4, 0xEE);                                \
    (r5) = _mm512_shuffle_f64x2(tt2, tt6, 0xEE);                                \
    (r6) = _mm512_shuffle_f64x2(tt1, tt5, 0xEE);                                \
    (r7) = _mm512_shuffle_f64x2(tt3, tt7, 0xEE);                                \
} while(0)

#define transpose_512(a,b,c,d,e,f,g,h,i,j) do { \
    transpose8x8_pd_512(a,b,c,d,e,f,g,h);       \
    i = a; j = b;                               \
} while(0)

// ============================================================
// Input_Output 宏 (AVX-512, 8 lane 版本)
// ============================================================

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
// 对应 AVX-256 中的 loadv_3d_x_m_blk / loadv_3d_x_c_blk / loadv_3d_x_p_blk / vstore_3d_x_pp_blk
// 使用 VVECLEN=8 代替 VECLEN=4
// ============================================================

#define loadv_512_3d_x_m_blk(y, z)  _mm512_loadu_pd(&BV0[ (y) - (myybeg - VVECLEN + 1) ][ (z) - (myzbeg - VVECLEN + 1) ][ 0 ])
#define loadv_512_3d_x_c_blk(y, z)  _mm512_loadu_pd(&BV1[ (y) - (myybeg - VVECLEN + 1) ][ (z) - (myzbeg - VVECLEN + 1) ][ 0 ])
#define loadv_512_3d_x_p_blk(y, z)  _mm512_loadu_pd(&BV2[ (y) - (myybeg - VVECLEN + 1) ][ (z) - (myzbeg - VVECLEN + 1) ][ 0 ])
#define vstore_512_3d_x_pp_blk(v, y, z) _mm512_storeu_pd(&BV3[ (y) - (myybeg - VVECLEN + 1) ][ (z) - (myzbeg - VVECLEN + 1) ][ 0 ], v)

// ============================================================
// 3D 向量 set / store 宏 (AVX-512 边界处理)
// _mm512_set_pd 参数从高位到低位排列
// ============================================================

#define setv_3d_blk_512(x, y, z) \
    _mm512_set_pd(B[(t+1)%2][(x)           ][(y) + 0][(z) + 0], \
                  B[(t)%2  ][(x) + STRIDE  ][(y) + 1][(z) + 1], \
                  B[(t+1)%2][(x) + STRIDE*2][(y) + 2][(z) + 2], \
                  B[(t)%2  ][(x) + STRIDE*3][(y) + 3][(z) + 3], \
                  B[(t+1)%2][(x) + STRIDE*4][(y) + 4][(z) + 4], \
                  B[(t)%2  ][(x) + STRIDE*5][(y) + 5][(z) + 5], \
                  B[(t+1)%2][(x) + STRIDE*6][(y) + 6][(z) + 6], \
                  B[(t)%2  ][(x) + STRIDE*7][(y) + 7][(z) + 7])

#define vset_3d_blk_512(vec, x, y, z) (vec) = setv_3d_blk_512((x), (y), (z))

#define vstore_set_3d_blk_512(x, y, z, v) do { \
    double tmp_512[8]; \
    _mm512_storeu_pd(tmp_512, (v)); \
    B[(t+1)%2][(x) + STRIDE * 7][(y) + 7][(z) + 7] = tmp_512[0]; \
    B[(t)%2  ][(x) + STRIDE * 6][(y) + 6][(z) + 6] = tmp_512[1]; \
    B[(t+1)%2][(x) + STRIDE * 5][(y) + 5][(z) + 5] = tmp_512[2]; \
    B[(t)%2  ][(x) + STRIDE * 4][(y) + 4][(z) + 4] = tmp_512[3]; \
    B[(t+1)%2][(x) + STRIDE * 3][(y) + 3][(z) + 3] = tmp_512[4]; \
    B[(t)%2  ][(x) + STRIDE * 2][(y) + 2][(z) + 2] = tmp_512[5]; \
    B[(t+1)%2][(x) + STRIDE * 1][(y) + 1][(z) + 1] = tmp_512[6]; \
    B[(t)%2  ][(x)             ][(y) + 0][(z) + 0] = tmp_512[7]; \
} while(0)

// ============================================================
// Blocking 加载宏 (AVX-512)
// 对应 AVX-256: load_x_m_blocking / load_x_p_blocking / load_x_c_blocking / load_v / store_v / store_x_pp
// ============================================================

#define load_x_m_blocking_512(x, y, z, yshift, zshift) loadv_512_3d_x_m_blk((y) + (yshift), (z) + (zshift))
#define load_x_p_blocking_512(x, y, z, yshift, zshift) loadv_512_3d_x_p_blk((y) + (yshift), (z) + (zshift))
#define load_x_c_blocking_512(x, y, z, yshift, zshift) loadv_512_3d_x_c_blk((y) + (yshift), (z) + (zshift))

#define load_v_512(x, y, z, xshift, yshift, zshift)        setv_3d_blk_512((x) + (xshift), (y) + (yshift), (z) + (zshift))
#define store_v_512(v, x, y, z, xshift, yshift, zshift)    vstore_set_3d_blk_512((x) + (xshift), (y) + (yshift), (z) + (zshift), v)

#define store_x_pp_512(v, x, y, z, yshift, zshift)         vstore_512_3d_x_pp_blk(v, (y) + (yshift), (z) + (zshift))

// ============================================================
// 27p 系数和计算宏 (AVX-512 版本)
// 与 defines.h 中的 SET_COFF / Compute_1vector 接口完全一致，仅使用 __m512d
// ============================================================

#ifdef simplestencil

#define SET_COFF_512  __m512d vc1_512;\
                      vallset_512(vc1_512, C1)

#define Compute_1vector_512(v_center_0, v_center_1, v_center_2, v_center_3, \
                            v_all_d_1_0, v_all_d_1_1, v_all_d_1_2, \
                            v_all_d_2_0, v_all_d_2_1, v_all_d_2_2)  \
    v_center_0 = _mm512_mul_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(\
                 _mm512_add_pd(v_center_0, v_center_1), v_center_2), v_all_d_1_0), v_all_d_1_1), v_all_d_1_2), v_all_d_2_0), v_all_d_2_1), v_all_d_2_2),\
                 vc1_512)

#else

#define SET_COFF_512  __m512d vc1_512, vc0_512, vc2_512, vc3_512;\
                      vallset_512(vc1_512, C1);\
                      vallset_512(vc0_512, C0);\
                      vallset_512(vc2_512, C2);\
                      vallset_512(vc3_512, C3)

#define Compute_1vector_512(v_center_0, v_center_1, v_center_2, v_center_3, \
                            v_all_d_1_0, v_all_d_1_1, v_all_d_1_2, \
                            v_all_d_2_0, v_all_d_2_1, v_all_d_2_2)  \
    v_center_0 = _mm512_fmadd_pd(vc3_512,\
                 _mm512_add_pd(_mm512_add_pd(v_all_d_2_1, v_all_d_1_0), v_all_d_1_2),\
                 _mm512_fmadd_pd(vc1_512,\
                                 _mm512_add_pd(_mm512_add_pd(v_center_0, v_all_d_1_1), v_center_2),\
                                 _mm512_fmadd_pd(vc2_512,\
                                                 _mm512_add_pd(v_all_d_2_0, v_all_d_2_2),\
                                                 _mm512_mul_pd(vc0_512, v_center_1))))
#endif

// ============================================================
// 27p Add_4 宏 (AVX-512 版本)
// ============================================================

#define Add_4_vectors_512(a, b, c, d) _mm512_add_pd(\
                                       _mm512_add_pd(\
                                       _mm512_add_pd(a, b), c), d)

#define Add_4_d_1_512(x, y, z, a) Add_4_vectors_512(    load_x_m_blocking_512(x, y, z, 0, a),\
                                                        load_x_p_blocking_512(x, y, z, 0, a),\
                                                        ( y == myybeg - VVECLEN + 1 + myyb - 1 ) ? load_v_512(x, y, z, 0, 1, a)  : load_x_c_blocking_512(x, y, z, 1, a),\
                                                        ( y == myybeg - VVECLEN + 1 )            ? load_v_512(x, y, z, 0, -1, a) : load_x_c_blocking_512(x, y, z, -1, a) )

#define Add_4_d_2_512(x, y, z, a) Add_4_vectors_512(    ( y == myybeg - VVECLEN + 1 + myyb - 1 ) ? load_v_512(x, y, z, -1, 1, a)  : load_x_m_blocking_512(x, y, z, 1, a),\
                                                        ( y == myybeg - VVECLEN + 1 )            ? load_v_512(x, y, z, -1, -1, a) : load_x_m_blocking_512(x, y, z, -1, a),\
                                                        ( y == myybeg - VVECLEN + 1 + myyb - 1 ) ? load_v_512(x, y, z, 1, 1, a)   : load_x_p_blocking_512(x, y, z, 1, a),\
                                                        ( y == myybeg - VVECLEN + 1 )            ? load_v_512(x, y, z, 1, -1, a)  : load_x_p_blocking_512(x, y, z, -1, a) )

// ============================================================
// 函数声明
// ============================================================

void blocking_parallel_rectangle_vectime_extra_array_512(double *A, int NX, int NY, int NZ, int T, int xb, int yb, int zb, int tb);

#endif /* DEFINE_512_H */
