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

void naive_vector(double * A, int NX, int NY, int T, int xb, int yb, int tb);
void blocking_parallel_rectangle_scalar(double * A, int NX, int NY, int T, int xb, int yb, int tb);
void blocking_parallel_rectangle_vector(double * A, int NX, int NY, int T, int xb, int yb, int tb);
void blocking_parallel_rectangle_vectime_extra_array(double * A, int NX, int NY, int T, int xb, int yb, int tb);
int checkresult(int NX, int NY, double (* A_correct)[NY + 2 * YSTART],
				double (* A)[NY + 2 * YSTART]);

typedef __m256d vec;

#define load_sum_from_bv(row_c, row_d1, row_d2, yy) {\
		int y0 = (yy);\
		vload(row_c, BV2[y0 - YSTART][0]);\
		vload(tmp_left, BV1[y0 - YSTART][0]);\
		vload(tmp_right, BV3[y0 - YSTART][0]);\
		row_d1 = _mm256_add_pd(tmp_left, tmp_right);\
		vload(tmp_left, BV0[y0 - YSTART][0]);\
		vload(tmp_right, BV4[y0 - YSTART][0]);\
		row_d2 = _mm256_add_pd(tmp_left, tmp_right);\
	}

#define load_sum_direct(row_c, row_d1, row_d2, yy) {\
		int y0 = (yy);\
		vloadset(row_c,	B[(t+1)%2][x                 ][y0],\
							B[t%2    ][x     + STRIDE    ][y0],\
							B[(t+1)%2][x     + STRIDE * 2][y0],\
							B[t%2    ][x     + STRIDE * 3][y0]);\
		vloadset(tmp_left,	B[(t+1)%2][x - 1             ][y0],\
							B[t%2    ][x - 1 + STRIDE    ][y0],\
							B[(t+1)%2][x - 1 + STRIDE * 2][y0],\
							B[t%2    ][x - 1 + STRIDE * 3][y0]);\
		vloadset(tmp_right,	B[(t+1)%2][x + 1             ][y0],\
							B[t%2    ][x + 1 + STRIDE    ][y0],\
							B[(t+1)%2][x + 1 + STRIDE * 2][y0],\
							B[t%2    ][x + 1 + STRIDE * 3][y0]);\
		row_d1 = _mm256_add_pd(tmp_left, tmp_right);\
		vloadset(tmp_left,	B[(t+1)%2][x - 2             ][y0],\
							B[t%2    ][x - 2 + STRIDE    ][y0],\
							B[(t+1)%2][x - 2 + STRIDE * 2][y0],\
							B[t%2    ][x - 2 + STRIDE * 3][y0]);\
		vloadset(tmp_right,	B[(t+1)%2][x + 2             ][y0],\
							B[t%2    ][x + 2 + STRIDE    ][y0],\
							B[(t+1)%2][x + 2 + STRIDE * 2][y0],\
							B[t%2    ][x + 2 + STRIDE * 3][y0]);\
		row_d2 = _mm256_add_pd(tmp_left, tmp_right);\
	}

#define Compute_1vector_25p(\
		m2_c, m2_d1, m2_d2,\
		m1_c, m1_d1, m1_d2,\
		mid_c, mid_d1, mid_d2,\
		p1_c, p1_d1, p1_d2,\
		p2_c, p2_d1, p2_d2) {\
		v_result = _mm256_mul_pd(vc00, mid_c);\
		sum_c01 = _mm256_add_pd(mid_d1, m1_c);\
		sum_c01 = _mm256_add_pd(sum_c01, p1_c);\
		v_result = _mm256_fmadd_pd(vc01, sum_c01, v_result);\
		sum_c02 = _mm256_add_pd(mid_d2, m2_c);\
		sum_c02 = _mm256_add_pd(sum_c02, p2_c);\
		v_result = _mm256_fmadd_pd(vc02, sum_c02, v_result);\
		sum_c11 = _mm256_add_pd(m1_d1, p1_d1);\
		v_result = _mm256_fmadd_pd(vc11, sum_c11, v_result);\
		sum_c22 = _mm256_add_pd(m2_d2, p2_d2);\
		v_result = _mm256_fmadd_pd(vc22, sum_c22, v_result);\
		sum_c12 = _mm256_add_pd(m2_d1, p2_d1);\
		sum_d2_pair = _mm256_add_pd(m1_d2, p1_d2);\
		sum_c12 = _mm256_add_pd(sum_c12, sum_d2_pair);\
		v_result = _mm256_fmadd_pd(vc12, sum_c12, v_result);\
	}
