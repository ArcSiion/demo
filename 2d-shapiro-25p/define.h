#include "../common.h"

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
