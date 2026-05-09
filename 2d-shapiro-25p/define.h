#include "../common.h"

#undef vload
#undef vstore
#define vload(a,b) a=_mm256_load_pd((&b))
#define vstore(a,b) _mm256_store_pd((&a),b)

#define RADIUS 2
#define STRIDE 3

#define XSTART RADIUS
#define YSTART 4
#define XSLOPE 2
#define YSLOPE 2

#define INIT 1.0 * (rand() % 1024)

#define SHAP_MU 1.0

#define W0 (1.0 - 3.0 * SHAP_MU / 8.0)
#define W1 (SHAP_MU / 4.0)
#define W2 (-SHAP_MU / 16.0)

#define C00 (W0 * W0)
#define C01 (W0 * W1)
#define C02 (W0 * W2)
#define C11 (W1 * W1)
#define C22 (W2 * W2)
#define C12 (W1 * W2)

#define SET_COFF  __m256d vc00 = _mm256_set1_pd(C00);\
				   __m256d vc01 = _mm256_set1_pd(C01);\
				   __m256d vc02 = _mm256_set1_pd(C02);\
				   __m256d vc11 = _mm256_set1_pd(C11);\
				   __m256d vc22 = _mm256_set1_pd(C22);\
				   __m256d vc12 = _mm256_set1_pd(C12)

//=====================================

// 从 BV0..BV4 读取一个 y 行，并提前合成 c/h1/h2 三个 feature。
#define load_feature_row_from_bv(row, yy) {\
		int y0 = (yy);\
		vload(row##_c, BV2[y0 - YSTART][0]);\
		vload(v_left_tmp, BV1[y0 - YSTART][0]);\
		vload(v_right_tmp, BV3[y0 - YSTART][0]);\
		row##_h1 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
		vload(v_left_tmp, BV0[y0 - YSTART][0]);\
		vload(v_right_tmp, BV4[y0 - YSTART][0]);\
		row##_h2 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
	}

// 从普通 B 数组直接 gather 一个 y 行，供 y 边界和 y tail 使用。
#define load_feature_row_direct(row, yy) {\
		int y0 = (yy);\
		vloadset(row##_c,	B[(t+1)%2][x                 ][y0],\
							B[t%2    ][x     + STRIDE    ][y0],\
							B[(t+1)%2][x     + STRIDE * 2][y0],\
							B[t%2    ][x     + STRIDE * 3][y0]);\
		vloadset(v_left_tmp,	B[(t+1)%2][x - 1             ][y0],\
							B[t%2    ][x - 1 + STRIDE    ][y0],\
							B[(t+1)%2][x - 1 + STRIDE * 2][y0],\
							B[t%2    ][x - 1 + STRIDE * 3][y0]);\
		vloadset(v_right_tmp,	B[(t+1)%2][x + 1             ][y0],\
							B[t%2    ][x + 1 + STRIDE    ][y0],\
							B[(t+1)%2][x + 1 + STRIDE * 2][y0],\
							B[t%2    ][x + 1 + STRIDE * 3][y0]);\
		row##_h1 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
		vloadset(v_left_tmp,	B[(t+1)%2][x - 2             ][y0],\
							B[t%2    ][x - 2 + STRIDE    ][y0],\
							B[(t+1)%2][x - 2 + STRIDE * 2][y0],\
							B[t%2    ][x - 2 + STRIDE * 3][y0]);\
		vloadset(v_right_tmp,	B[(t+1)%2][x + 2             ][y0],\
							B[t%2    ][x + 2 + STRIDE    ][y0],\
							B[(t+1)%2][x + 2 + STRIDE * 2][y0],\
							B[t%2    ][x + 2 + STRIDE * 3][y0]);\
		row##_h2 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
	}

// 使用 5 个 feature row 计算一个 25p 输出时间向量。
// 计算分组顺序为 C00, C01, C02, C11, C22, C12。
// 宏体内部不能使用 // 注释加反斜杠续行，否则会破坏宏展开。
#define compute_25p_feature_window(row_m2, row_m1, row_0, row_p1, row_p2) {\
		v_result = _mm256_mul_pd(vc00, row_0##_c);\
		v_c01_sum = _mm256_add_pd(row_0##_h1, row_m1##_c);\
		v_c01_sum = _mm256_add_pd(v_c01_sum, row_p1##_c);\
		v_result = _mm256_fmadd_pd(vc01, v_c01_sum, v_result);\
		v_c02_sum = _mm256_add_pd(row_0##_h2, row_m2##_c);\
		v_c02_sum = _mm256_add_pd(v_c02_sum, row_p2##_c);\
		v_result = _mm256_fmadd_pd(vc02, v_c02_sum, v_result);\
		v_c11_sum = _mm256_add_pd(row_m1##_h1, row_p1##_h1);\
		v_result = _mm256_fmadd_pd(vc11, v_c11_sum, v_result);\
		v_c22_sum = _mm256_add_pd(row_m2##_h2, row_p2##_h2);\
		v_result = _mm256_fmadd_pd(vc22, v_c22_sum, v_result);\
		v_c12_sum = _mm256_add_pd(row_m2##_h1, row_p2##_h1);\
		v_pair_tmp = _mm256_add_pd(row_m1##_h2, row_p1##_h2);\
		v_c12_sum = _mm256_add_pd(v_c12_sum, v_pair_tmp);\
		v_result = _mm256_fmadd_pd(vc12, v_c12_sum, v_result);\
	}

//=====================================================

#define Compute_1vector(v_m2_m2, v_m1_m2, v_0_m2, v_p1_m2, v_p2_m2,\
						v_m2_m1, v_m1_m1, v_0_m1, v_p1_m1, v_p2_m1,\
						v_m2_0,  v_m1_0,  v_0_0,  v_p1_0,  v_p2_0,\
						v_m2_p1, v_m1_p1, v_0_p1, v_p1_p1, v_p2_p1,\
						v_m2_p2, v_m1_p2, v_0_p2, v_p1_p2, v_p2_p2,\
						vout) vout = \
		_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(\
		_mm256_mul_pd(vc00, v_0_0),\
		_mm256_mul_pd(vc01, _mm256_add_pd(_mm256_add_pd(_mm256_add_pd(v_m1_0, v_p1_0), v_0_m1), v_0_p1))),\
		_mm256_mul_pd(vc02, _mm256_add_pd(_mm256_add_pd(_mm256_add_pd(v_m2_0, v_p2_0), v_0_m2), v_0_p2))),\
		_mm256_mul_pd(vc11, _mm256_add_pd(_mm256_add_pd(_mm256_add_pd(v_m1_m1, v_p1_m1), v_m1_p1), v_p1_p1))),\
		_mm256_mul_pd(vc22, _mm256_add_pd(_mm256_add_pd(_mm256_add_pd(v_m2_m2, v_p2_m2), v_m2_p2), v_p2_p2))),\
		_mm256_mul_pd(vc12, _mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(_mm256_add_pd(v_m1_m2, v_p1_m2), v_m1_p2), v_p1_p2), v_m2_m1), v_m2_p1), v_p2_m1), v_p2_p1)))

#define Compute_scalar(A, t, x, y) A[((t) + 1) % 2][x][y] = \
		C00 * A[(t) % 2][x][y] + \
		C01 * ( \
			A[(t) % 2][(x) - 1][y] + \
			A[(t) % 2][(x) + 1][y] + \
			A[(t) % 2][x][(y) - 1] + \
			A[(t) % 2][x][(y) + 1] \
		) + \
		C02 * ( \
			A[(t) % 2][(x) - 2][y] + \
			A[(t) % 2][(x) + 2][y] + \
			A[(t) % 2][x][(y) - 2] + \
			A[(t) % 2][x][(y) + 2] \
		) + \
		C11 * ( \
			A[(t) % 2][(x) - 1][(y) - 1] + \
			A[(t) % 2][(x) + 1][(y) - 1] + \
			A[(t) % 2][(x) - 1][(y) + 1] + \
			A[(t) % 2][(x) + 1][(y) + 1] \
		) + \
		C22 * ( \
			A[(t) % 2][(x) - 2][(y) - 2] + \
			A[(t) % 2][(x) + 2][(y) - 2] + \
			A[(t) % 2][(x) - 2][(y) + 2] + \
			A[(t) % 2][(x) + 2][(y) + 2] \
		) + \
		C12 * ( \
			A[(t) % 2][(x) - 1][(y) - 2] + \
			A[(t) % 2][(x) + 1][(y) - 2] + \
			A[(t) % 2][(x) - 1][(y) + 2] + \
			A[(t) % 2][(x) + 1][(y) + 2] + \
			A[(t) % 2][(x) - 2][(y) - 1] + \
			A[(t) % 2][(x) - 2][(y) + 1] + \
			A[(t) % 2][(x) + 2][(y) - 1] + \
			A[(t) % 2][(x) + 2][(y) + 1] \
		)

void naive_scalar(double * A, int NX, int NY, int T);
void naive_vector(double * A, int NX, int NY, int T);
void vectime(double* A, int NX, int NY, int T);
int checkresult( int NX, int NY, double (* A_correct)[ NY+2*YSTART], double (* A)[ NY+2*YSTART]);

typedef __m256d vec;
