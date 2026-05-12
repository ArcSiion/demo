#include "define.h"

#ifndef __AVX512F__
#error "define_512.h requires AVX-512F. Compile with -mavx512f."
#endif

typedef __m512d vec_512;

#define vload_512(a,b) (a) = _mm512_load_pd((&b))
#define vstore_512(a,b) _mm512_store_pd((&a),(b))

#define vloadset_512(a, h, g, f, e, d, c, b, in_a) {\
		(a) = _mm512_set_pd(h, g, f, e, d, c, b, in_a);\
	}

#define alloc_extra_array_512(size) ({\
		void *ptr = NULL;\
		if (posix_memalign(&ptr, 64, size) != 0) { ptr = NULL; }\
		ptr;\
	})

#define free_extra_array_512(a) free_extra_array(a)

#define vrotate_512_low2high(a) \
	_mm512_castsi512_pd(_mm512_alignr_epi64(\
		_mm512_castpd_si512(a), _mm512_castpd_si512(a), 1))

#define vrotate_512_high2low(a) \
	_mm512_castsi512_pd(_mm512_alignr_epi64(\
		_mm512_castpd_si512(a), _mm512_castpd_si512(a), 7))

#define transpose8x8_pd_512(r0,r1,r2,r3,r4,r5,r6,r7) do {\
		__m512d t0 = _mm512_unpacklo_pd((r0),(r1));\
		__m512d t1 = _mm512_unpackhi_pd((r0),(r1));\
		__m512d t2 = _mm512_unpacklo_pd((r2),(r3));\
		__m512d t3 = _mm512_unpackhi_pd((r2),(r3));\
		__m512d t4 = _mm512_unpacklo_pd((r4),(r5));\
		__m512d t5 = _mm512_unpackhi_pd((r4),(r5));\
		__m512d t6 = _mm512_unpacklo_pd((r6),(r7));\
		__m512d t7 = _mm512_unpackhi_pd((r6),(r7));\
		const __m512i idx0 = _mm512_setr_epi64(0, 1, 8, 9, 4, 5, 12, 13);\
		const __m512i idx1 = _mm512_setr_epi64(2, 3, 10, 11, 6, 7, 14, 15);\
		__m512d tt0 = _mm512_permutex2var_pd(t0, idx0, t2);\
		__m512d tt1 = _mm512_permutex2var_pd(t0, idx1, t2);\
		__m512d tt2 = _mm512_permutex2var_pd(t1, idx0, t3);\
		__m512d tt3 = _mm512_permutex2var_pd(t1, idx1, t3);\
		__m512d tt4 = _mm512_permutex2var_pd(t4, idx0, t6);\
		__m512d tt5 = _mm512_permutex2var_pd(t4, idx1, t6);\
		__m512d tt6 = _mm512_permutex2var_pd(t5, idx0, t7);\
		__m512d tt7 = _mm512_permutex2var_pd(t5, idx1, t7);\
		(r0) = _mm512_shuffle_f64x2(tt0, tt4, 0x44);\
		(r1) = _mm512_shuffle_f64x2(tt2, tt6, 0x44);\
		(r2) = _mm512_shuffle_f64x2(tt1, tt5, 0x44);\
		(r3) = _mm512_shuffle_f64x2(tt3, tt7, 0x44);\
		(r4) = _mm512_shuffle_f64x2(tt0, tt4, 0xEE);\
		(r5) = _mm512_shuffle_f64x2(tt2, tt6, 0xEE);\
		(r6) = _mm512_shuffle_f64x2(tt1, tt5, 0xEE);\
		(r7) = _mm512_shuffle_f64x2(tt3, tt7, 0xEE);\
	} while(0)

#define transpose_512(a,b,c,d,e,f,g,h,in,out) do {\
		transpose8x8_pd_512(a,b,c,d,e,f,g,h);\
	} while(0)

#define Input_Output_1_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = (v1);\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_2_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x02, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_3_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x04, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_4_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x08, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_5_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x10, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_6_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x20, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_7_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x40, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
		(in) = vrotate_512_low2high(in);\
	} while(0)

#define Input_Output_8_512(out, v1, in) do {\
		(v1) = vrotate_512_high2low(v1);\
		(out) = _mm512_mask_blend_pd(\
			0x80, (out), _mm512_broadcastsd_pd(_mm512_castpd512_pd128(v1)));\
		(v1) = _mm512_mask_blend_pd(0x01, (v1), (in));\
	} while(0)

#define SET_COFF_512  vec_512 vc00_512 = _mm512_set1_pd(C00);\
					  vec_512 vc01_512 = _mm512_set1_pd(C01);\
					  vec_512 vc02_512 = _mm512_set1_pd(C02);\
					  vec_512 vc11_512 = _mm512_set1_pd(C11);\
					  vec_512 vc22_512 = _mm512_set1_pd(C22);\
					  vec_512 vc12_512 = _mm512_set1_pd(C12)

//=====================================

// 从 BV0..BV4 读取一个 y 行，并合成 c/d1/d2 三个横向和。
#define load_sum_from_bv_512(row_c, row_d1, row_d2, yy) {\
		int y0 = (yy);\
		vload_512(row_c, BV2[y0 - YSTART][0]);\
		vload_512(tmp_left, BV1[y0 - YSTART][0]);\
		vload_512(tmp_right, BV3[y0 - YSTART][0]);\
		row_d1 = _mm512_add_pd(tmp_left, tmp_right);\
		vload_512(tmp_left, BV0[y0 - YSTART][0]);\
		vload_512(tmp_right, BV4[y0 - YSTART][0]);\
		row_d2 = _mm512_add_pd(tmp_left, tmp_right);\
	}

// 从普通 B 数组直接 gather 一个 y 行，供 y 边界和 y tail 使用。
#define load_sum_direct_512(row_c, row_d1, row_d2, yy) {\
		int y0 = (yy);\
		vloadset_512(row_c,	B[(t+1)%2][x                 ][y0],\
							B[t%2    ][x     + STRIDE    ][y0],\
							B[(t+1)%2][x     + STRIDE * 2][y0],\
							B[t%2    ][x     + STRIDE * 3][y0],\
							B[(t+1)%2][x     + STRIDE * 4][y0],\
							B[t%2    ][x     + STRIDE * 5][y0],\
							B[(t+1)%2][x     + STRIDE * 6][y0],\
							B[t%2    ][x     + STRIDE * 7][y0]);\
		vloadset_512(tmp_left,	B[(t+1)%2][x - 1             ][y0],\
								B[t%2    ][x - 1 + STRIDE    ][y0],\
								B[(t+1)%2][x - 1 + STRIDE * 2][y0],\
								B[t%2    ][x - 1 + STRIDE * 3][y0],\
								B[(t+1)%2][x - 1 + STRIDE * 4][y0],\
								B[t%2    ][x - 1 + STRIDE * 5][y0],\
								B[(t+1)%2][x - 1 + STRIDE * 6][y0],\
								B[t%2    ][x - 1 + STRIDE * 7][y0]);\
		vloadset_512(tmp_right,	B[(t+1)%2][x + 1             ][y0],\
								B[t%2    ][x + 1 + STRIDE    ][y0],\
								B[(t+1)%2][x + 1 + STRIDE * 2][y0],\
								B[t%2    ][x + 1 + STRIDE * 3][y0],\
								B[(t+1)%2][x + 1 + STRIDE * 4][y0],\
								B[t%2    ][x + 1 + STRIDE * 5][y0],\
								B[(t+1)%2][x + 1 + STRIDE * 6][y0],\
								B[t%2    ][x + 1 + STRIDE * 7][y0]);\
		row_d1 = _mm512_add_pd(tmp_left, tmp_right);\
		vloadset_512(tmp_left,	B[(t+1)%2][x - 2             ][y0],\
								B[t%2    ][x - 2 + STRIDE    ][y0],\
								B[(t+1)%2][x - 2 + STRIDE * 2][y0],\
								B[t%2    ][x - 2 + STRIDE * 3][y0],\
								B[(t+1)%2][x - 2 + STRIDE * 4][y0],\
								B[t%2    ][x - 2 + STRIDE * 5][y0],\
								B[(t+1)%2][x - 2 + STRIDE * 6][y0],\
								B[t%2    ][x - 2 + STRIDE * 7][y0]);\
		vloadset_512(tmp_right,	B[(t+1)%2][x + 2             ][y0],\
								B[t%2    ][x + 2 + STRIDE    ][y0],\
								B[(t+1)%2][x + 2 + STRIDE * 2][y0],\
								B[t%2    ][x + 2 + STRIDE * 3][y0],\
								B[(t+1)%2][x + 2 + STRIDE * 4][y0],\
								B[t%2    ][x + 2 + STRIDE * 5][y0],\
								B[(t+1)%2][x + 2 + STRIDE * 6][y0],\
								B[t%2    ][x + 2 + STRIDE * 7][y0]);\
		row_d2 = _mm512_add_pd(tmp_left, tmp_right);\
	}

#define Compute_1vector_25p_512(\
		m2_c, m2_d1, m2_d2,\
		m1_c, m1_d1, m1_d2,\
		mid_c, mid_d1, mid_d2,\
		p1_c, p1_d1, p1_d2,\
		p2_c, p2_d1, p2_d2) {\
		v_result = _mm512_mul_pd(vc00_512, mid_c);\
		sum_c01 = _mm512_add_pd(mid_d1, m1_c);\
		sum_c01 = _mm512_add_pd(sum_c01, p1_c);\
		v_result = _mm512_fmadd_pd(vc01_512, sum_c01, v_result);\
		sum_c02 = _mm512_add_pd(mid_d2, m2_c);\
		sum_c02 = _mm512_add_pd(sum_c02, p2_c);\
		v_result = _mm512_fmadd_pd(vc02_512, sum_c02, v_result);\
		sum_c11 = _mm512_add_pd(m1_d1, p1_d1);\
		v_result = _mm512_fmadd_pd(vc11_512, sum_c11, v_result);\
		sum_c22 = _mm512_add_pd(m2_d2, p2_d2);\
		v_result = _mm512_fmadd_pd(vc22_512, sum_c22, v_result);\
		sum_c12 = _mm512_add_pd(m2_d1, p2_d1);\
		sum_d2_pair = _mm512_add_pd(m1_d2, p1_d2);\
		sum_c12 = _mm512_add_pd(sum_c12, sum_d2_pair);\
		v_result = _mm512_fmadd_pd(vc12_512, sum_c12, v_result);\
	}

void vectime_transpose_boundary_extra_array_512(double* A, int NX, int NY, int T);
