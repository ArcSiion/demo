#include "define.h"

#define load_time_vector(v, dx, yy) do {\
		int _yy = (yy);\
		if (_yy >= YSTART && _yy <= last_transposed_y) {\
			int _base_y = YSTART + ((_yy - YSTART) / VECLEN) * VECLEN;\
			int _lane_y = _yy - _base_y;\
			if (_lane_y == 0) {\
				vload(v, B[(t+1)%2][x + (dx)][_base_y]);\
			} else if (_lane_y == 1) {\
				vload(v, B[t%2    ][x + (dx) + STRIDE][_base_y]);\
			} else if (_lane_y == 2) {\
				vload(v, B[(t+1)%2][x + (dx) + STRIDE * 2][_base_y]);\
			} else {\
				vload(v, B[t%2    ][x + (dx) + STRIDE * 3][_base_y]);\
			}\
		} else {\
			vloadset(v, B[(t+1)%2][x + (dx)             ][_yy],\
						B[t%2    ][x + (dx) + STRIDE    ][_yy],\
						B[(t+1)%2][x + (dx) + STRIDE * 2][_yy],\
						B[t%2    ][x + (dx) + STRIDE * 3][_yy]);\
		}\
	} while (0)

#define load_25p_row(yy) do {\
		load_time_vector(v_x_minus2, -2, (yy));\
		load_time_vector(v_x_minus,  -1, (yy));\
		load_time_vector(v_center,    0, (yy));\
		load_time_vector(v_x_plus,    1, (yy));\
		load_time_vector(v_x_plus2,   2, (yy));\
	} while (0)

#define Compute_25p_stage1(yy) do {\
		load_25p_row((yy) - 2);\
		v_sum02 = v_center;\
		v_sum22 = _mm256_add_pd(v_x_minus2, v_x_plus2);\
		v_sum12 = _mm256_add_pd(v_x_minus, v_x_plus);\
	} while (0)

#define Compute_25p_stage2(yy) do {\
		load_25p_row((yy) - 1);\
		v_0_m1 = v_center;\
		v_sum11 = _mm256_add_pd(v_x_minus, v_x_plus);\
		v_m2_m1 = v_x_minus2;\
		v_p2_m1 = v_x_plus2;\
		load_25p_row(yy);\
		vout = _mm256_mul_pd(vc00, v_center);\
		v_sum01 = _mm256_add_pd(v_x_minus, v_x_plus);\
		v_sum01 = _mm256_add_pd(v_sum01, v_0_m1);\
		v_sum02 = _mm256_add_pd(_mm256_add_pd(v_x_minus2, v_x_plus2), v_sum02);\
		load_25p_row((yy) + 1);\
		v_sum01 = _mm256_add_pd(v_sum01, v_center);\
		v_sum11 = _mm256_add_pd(v_sum11, v_x_minus);\
		v_sum11 = _mm256_add_pd(v_sum11, v_x_plus);\
		v_m2_p1 = v_x_minus2;\
		v_p2_p1 = v_x_plus2;\
	} while (0)

#define Compute_25p_stage3(yy) do {\
		load_25p_row((yy) + 2);\
		v_sum02 = _mm256_add_pd(v_sum02, v_center);\
		v_sum22 = _mm256_add_pd(v_sum22, v_x_minus2);\
		v_sum22 = _mm256_add_pd(v_sum22, v_x_plus2);\
		v_sum12 = _mm256_add_pd(v_sum12, v_x_minus);\
		v_sum12 = _mm256_add_pd(v_sum12, v_x_plus);\
		v_sum12 = _mm256_add_pd(v_sum12, v_m2_m1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_m2_p1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_p2_m1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_p2_p1);\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc01, v_sum01));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc02, v_sum02));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc11, v_sum11));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc22, v_sum22));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc12, v_sum12));\
	} while (0)

#define Compute_25p_vector(yy) do {\
		Compute_25p_stage1(yy);\
		Compute_25p_stage2(yy);\
		Compute_25p_stage3(yy);\
	} while (0)

#define store_time_vector(yy) do {\
		_mm256_storeu_pd(tmp, vout);\
		B[(t+1)%2][x + STRIDE * 3][(yy)] = tmp[0];\
		B[t%2    ][x + STRIDE * 2][(yy)] = tmp[1];\
		B[(t+1)%2][x + STRIDE    ][(yy)] = tmp[2];\
		B[t%2    ][x             ][(yy)] = tmp[3];\
	} while (0)

void vectime(double* A, int NX, int NY, int T) {
	double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;
	double tmp[4];

	int tt, t = 0, x, xx, y;
	int x_vector_end;
	int last_transposed_y = YSTART - 1;
	vec v_center_0, v_center_1, v_center_2, v_center_3;
	vec v_x_minus2, v_x_minus, v_center, v_x_plus, v_x_plus2;
	vec v_sum01, v_sum02, v_sum11, v_sum22, v_sum12;
	vec v_0_m1, v_m2_m1, v_p2_m1, v_m2_p1, v_p2_p1;
	vec in, out, vout;
	SET_COFF;

	if (NY >= VECLEN) {
		last_transposed_y = YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN - 1;
	}

	/* x 方向太短时没有完整的 temporal-vector 主体，直接退回 scalar。 */
	if (NX < STRIDE * VECLEN) {
		for (t = 0; t < T; t++) {
			for (x = XSTART; x < NX + XSTART; x++) {
            #pragma ivdep
            #pragma vector always
				for (y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}
		return;
	}

	for (tt = 0; tt <= T - VECLEN; tt += VECLEN) {

		/* 和 2d9p 一样，先用 scalar 补出左侧阶梯形依赖区。 */
		for (t = tt; t < tt + VECLEN - 1; t++) {
			for (x = XSTART; x < XSTART + STRIDE * (VECLEN - 1 - (t - tt)); x++) {
            #pragma ivdep
            #pragma vector always
				for (y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}

		t = tt;
		x_vector_end = NX + XSTART - STRIDE * VECLEN;

		/* 初始 5 列原地转置：x-2 到 x+2，对应 Shapiro radius=2。 */
		for (x = XSTART - XSLOPE; x <= XSTART + XSLOPE; x++) {
			for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(v_center_0, B[t%2    ][x + STRIDE * 3][y]);
				vload(v_center_1, B[(t+1)%2][x + STRIDE * 2][y]);
				vload(v_center_2, B[t%2    ][x + STRIDE    ][y]);
				vload(v_center_3, B[(t+1)%2][x             ][y]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore(B[t%2    ][x + STRIDE * 3][y], v_center_3);
				vstore(B[(t+1)%2][x + STRIDE * 2][y], v_center_2);
				vstore(B[t%2    ][x + STRIDE    ][y], v_center_1);
				vstore(B[(t+1)%2][x             ][y], v_center_0);
			}
		}

		/* 主体循环基本照搬 2d9p：每次 x 前进 1，最右侧新列由当前输出 rotate/blend 得到。 */
		for (x = XSTART; x <= x_vector_end; x++) {
			for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(in, B[t%2][x + STRIDE * 4][y]);

				/* 三阶段加载当前 yy，组内加法顺序保持和 Compute_1vector 一致。 */
				Compute_25p_vector(y);
				Input_Output_1(out, vout, in);
				vstore(B[(t+1)%2][x + STRIDE][y], vout);

				Compute_25p_vector(y + 1);
				Input_Output_2(out, vout, in);
				vstore(B[t%2][x + STRIDE * 2][y], vout);

				Compute_25p_vector(y + 2);
				Input_Output_3(out, vout, in);
				vstore(B[(t+1)%2][x + STRIDE * 3][y], vout);

				Compute_25p_vector(y + 3);
				Input_Output_4(out, vout, in);
				vstore(B[t%2][x + STRIDE * 4][y], vout);

				vstore(B[t%2][x][y], out);
			}

			/* y 方向剩余点不再转置，使用直接 gather/scatter 保证逻辑清楚。 */
			for (y = last_transposed_y + 1; y < NY + YSTART; y++) {
				Compute_25p_vector(y);
				store_time_vector(y);
			}
		}

		xx = x;

		/* 将最后保留在原地转置布局中的 5 列转回普通布局。 */
		for (x = xx - XSLOPE; x <= xx + XSLOPE; x++) {
			for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(v_center_3, B[t%2    ][x + STRIDE * 3][y]);
				vload(v_center_2, B[(t+1)%2][x + STRIDE * 2][y]);
				vload(v_center_1, B[t%2    ][x + STRIDE    ][y]);
				vload(v_center_0, B[(t+1)%2][x             ][y]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore(B[t%2    ][x + STRIDE * 3][y], v_center_0);
				vstore(B[(t+1)%2][x + STRIDE * 2][y], v_center_1);
				vstore(B[t%2    ][x + STRIDE    ][y], v_center_2);
				vstore(B[(t+1)%2][x             ][y], v_center_3);
			}
		}

		/* 右侧阶梯形依赖区继续沿用 scalar 收尾。 */
		for (t = tt; t < tt + VECLEN; t++) {
			for (x = xx + STRIDE * (VECLEN - 1 - (t - tt)); x < NX + XSTART; x++) {
            #pragma ivdep
            #pragma vector always
				for (y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}

		/* 左 halo 被临时写成转置/中间值，时间 tile 结束后恢复。 */
		for (x = XSTART - XSLOPE; x < XSTART; x++) {
			for (y = 0; y < NY + YSTART * 2; y++) {
				B[1][x][y] = B[0][x][y];
			}
		}
	}

	for (; t < T; t++) {
		for (x = XSTART; x < NX + XSTART; x++) {
        #pragma ivdep
        #pragma vector always
			for (y = YSTART; y < NY + YSTART; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}
}

#undef store_time_vector
#undef Compute_25p_vector
#undef Compute_25p_stage3
#undef Compute_25p_stage2
#undef Compute_25p_stage1
#undef load_25p_row
#undef load_time_vector
