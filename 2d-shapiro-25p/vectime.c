#include "define.h"

#define load_time_vector(v, dx, yy) do {\
		int _yy = (yy);\
		if (_yy >= YSTART && _yy <= last_transposed_y) {\
			vload(v, BV[(dx) + XSLOPE][_yy - YSTART][0]);\
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

#define flush_pending_until(limit_y) do {\
		int _limit_y = (limit_y);\
		while (pending_count > 0 && pending_y0 <= _limit_y) {\
			vstore(BV[0][pending_y0 - YSTART][0], pending_vec0);\
			if (pending_count == 2) {\
				pending_vec0 = pending_vec1;\
				pending_y0 = pending_y1;\
				pending_count = 1;\
			} else {\
				pending_count = 0;\
			}\
		}\
	} while (0)

#define enqueue_new_right(yy) do {\
		if (pending_count == 0) {\
			pending_vec0 = vout;\
			pending_y0 = (yy);\
			pending_count = 1;\
		} else {\
			pending_vec1 = vout;\
			pending_y1 = (yy);\
			pending_count = 2;\
		}\
	} while (0)

void vectime(double* A, int NX, int NY, int T) {
	double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;
	double tmp[4];

	int tt, t = 0, x, xx, y;
	int x_vector_end;
	int bv_rows = (NY > 0) ? NY : 1;
	int last_transposed_y = YSTART - 1;
	int pending_y0 = 0, pending_y1 = 0, pending_count = 0;
	vec v_center_0, v_center_1, v_center_2, v_center_3;
	vec v_x_minus2, v_x_minus, v_center, v_x_plus, v_x_plus2;
	vec v_sum01, v_sum02, v_sum11, v_sum22, v_sum12;
	vec v_0_m1, v_m2_m1, v_p2_m1, v_m2_p1, v_p2_p1;
	vec pending_vec0, pending_vec1;
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

	double (* AV) [bv_rows][VECLEN] = (double(*)[bv_rows][VECLEN])alloc_extra_array(sizeof(double) * bv_rows * VECLEN * 5);
	double (* BV0) [VECLEN] = (double (*) [VECLEN]) AV;
	double (* BV1) [VECLEN] = (double (*) [VECLEN]) (AV + 1);
	double (* BV2) [VECLEN] = (double (*) [VECLEN]) (AV + 2);
	double (* BV3) [VECLEN] = (double (*) [VECLEN]) (AV + 3);
	double (* BV4) [VECLEN] = (double (*) [VECLEN]) (AV + 4);
	double (* BV[5]) [VECLEN] = {BV0, BV1, BV2, BV3, BV4};
	double (* BV_tmp) [VECLEN];

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
		BV[0] = BV0;
		BV[1] = BV1;
		BV[2] = BV2;
		BV[3] = BV3;
		BV[4] = BV4;

		/* 初始 5 列打包到 extra array：BV0..BV4 对应 x-2..x+2。 */
		for (x = XSTART - XSLOPE; x <= XSTART + XSLOPE; x++) {
			for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(v_center_0, B[t%2    ][x + STRIDE * 3][y]);
				vload(v_center_1, B[(t+1)%2][x + STRIDE * 2][y]);
				vload(v_center_2, B[t%2    ][x + STRIDE    ][y]);
				vload(v_center_3, B[(t+1)%2][x             ][y]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART) + 3][0], v_center_3);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART) + 2][0], v_center_2);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART) + 1][0], v_center_1);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART)    ][0], v_center_0);
			}
		}

		for (x = XSTART; x <= x_vector_end; x++) {
			pending_count = 0;

			for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(in, B[t%2][x + STRIDE * 4][y]);

				/* radius=2 还会在 y+1/y+2 使用旧 BV0[y]，所以写新右列时延迟两行。 */
				Compute_25p_vector(y);
				Input_Output_1(out, vout, in);
				flush_pending_until(y - YSLOPE);
				enqueue_new_right(y);

				Compute_25p_vector(y + 1);
				Input_Output_2(out, vout, in);
				flush_pending_until(y + 1 - YSLOPE);
				enqueue_new_right(y + 1);

				Compute_25p_vector(y + 2);
				Input_Output_3(out, vout, in);
				flush_pending_until(y + 2 - YSLOPE);
				enqueue_new_right(y + 2);

				Compute_25p_vector(y + 3);
				Input_Output_4(out, vout, in);
				flush_pending_until(y + 3 - YSLOPE);
				enqueue_new_right(y + 3);

				vstore(B[t%2][x][y], out);
			}

			/* y 尾部不进入 BV 主体，继续用 direct gather/scatter。 */
			for (y = last_transposed_y + 1; y < NY + YSTART; y++) {
				Compute_25p_vector(y);
				store_time_vector(y);
				flush_pending_until(y - YSLOPE);
			}
			flush_pending_until(last_transposed_y);

			/* 当前旧 BV0 已经被重写成新右列，轮转后成为下一轮的 BV4。 */
			BV_tmp = BV[0];
			BV[0] = BV[1];
			BV[1] = BV[2];
			BV[2] = BV[3];
			BV[3] = BV[4];
			BV[4] = BV_tmp;
		}

		xx = x;

		/* x 主体结束后，把最后 5 列从 extra array 转回普通 B 布局。 */
		for (x = xx - XSLOPE; x <= xx + XSLOPE; x++) {
			for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(v_center_3, BV[x - (xx - XSLOPE)][(y - YSTART) + 3][0]);
				vload(v_center_2, BV[x - (xx - XSLOPE)][(y - YSTART) + 2][0]);
				vload(v_center_1, BV[x - (xx - XSLOPE)][(y - YSTART) + 1][0]);
				vload(v_center_0, BV[x - (xx - XSLOPE)][(y - YSTART)    ][0]);
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

		/* extra array 不再改写左 halo，这里保留恢复动作，避免后续 tile 读到旧时间层差异。 */
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

	free_extra_array(AV);
}

#undef enqueue_new_right
#undef flush_pending_until
#undef store_time_vector
#undef Compute_25p_vector
#undef Compute_25p_stage3
#undef Compute_25p_stage2
#undef Compute_25p_stage1
#undef load_25p_row
#undef load_time_vector
