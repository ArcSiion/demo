#include "define.h"

#define load_row_bv(row, yy) {\
		int _load_y = (yy);\
		vload(row##_m2, BV[0][_load_y - YSTART][0]);\
		vload(row##_m1, BV[1][_load_y - YSTART][0]);\
		vload(row##_0,  BV[2][_load_y - YSTART][0]);\
		vload(row##_p1, BV[3][_load_y - YSTART][0]);\
		vload(row##_p2, BV[4][_load_y - YSTART][0]);\
	}

#define load_row_direct(row, yy) {\
		int _load_y = (yy);\
		vloadset(row##_m2,	B[(t+1)%2][x - 2             ][_load_y],\
							B[t%2    ][x - 2 + STRIDE    ][_load_y],\
							B[(t+1)%2][x - 2 + STRIDE * 2][_load_y],\
							B[t%2    ][x - 2 + STRIDE * 3][_load_y]);\
		vloadset(row##_m1,	B[(t+1)%2][x - 1             ][_load_y],\
							B[t%2    ][x - 1 + STRIDE    ][_load_y],\
							B[(t+1)%2][x - 1 + STRIDE * 2][_load_y],\
							B[t%2    ][x - 1 + STRIDE * 3][_load_y]);\
		vloadset(row##_0,	B[(t+1)%2][x                 ][_load_y],\
							B[t%2    ][x     + STRIDE    ][_load_y],\
							B[(t+1)%2][x     + STRIDE * 2][_load_y],\
							B[t%2    ][x     + STRIDE * 3][_load_y]);\
		vloadset(row##_p1,	B[(t+1)%2][x + 1             ][_load_y],\
							B[t%2    ][x + 1 + STRIDE    ][_load_y],\
							B[(t+1)%2][x + 1 + STRIDE * 2][_load_y],\
							B[t%2    ][x + 1 + STRIDE * 3][_load_y]);\
		vloadset(row##_p2,	B[(t+1)%2][x + 2             ][_load_y],\
							B[t%2    ][x + 2 + STRIDE    ][_load_y],\
							B[(t+1)%2][x + 2 + STRIDE * 2][_load_y],\
							B[t%2    ][x + 2 + STRIDE * 3][_load_y]);\
	}

#define load_row_boundary(row, yy) {\
		int _boundary_y = (yy);\
		if (_boundary_y >= YSTART && _boundary_y <= last_transposed_y) {\
			load_row_bv(row, _boundary_y);\
		} else {\
			load_row_direct(row, _boundary_y);\
		}\
	}

#define copy_row(dst, src) {\
		dst##_m2 = src##_m2;\
		dst##_m1 = src##_m1;\
		dst##_0  = src##_0;\
		dst##_p1 = src##_p1;\
		dst##_p2 = src##_p2;\
	}

#define shift_row_window() {\
		copy_row(row0, row1);\
		copy_row(row1, row2);\
		copy_row(row2, row3);\
		copy_row(row3, row4);\
	}

#define Compute_25p_window(row_m2, row_m1, row_0, row_p1, row_p2) {\
		v_sum02 = row_m2##_0;\
		v_sum22 = _mm256_add_pd(row_m2##_m2, row_m2##_p2);\
		v_sum12 = _mm256_add_pd(row_m2##_m1, row_m2##_p1);\
		v_0_m1 = row_m1##_0;\
		v_sum11 = _mm256_add_pd(row_m1##_m1, row_m1##_p1);\
		v_m2_m1 = row_m1##_m2;\
		v_p2_m1 = row_m1##_p2;\
		vout = _mm256_mul_pd(vc00, row_0##_0);\
		v_sum01 = _mm256_add_pd(row_0##_m1, row_0##_p1);\
		v_sum01 = _mm256_add_pd(v_sum01, v_0_m1);\
		v_sum02 = _mm256_add_pd(_mm256_add_pd(row_0##_m2, row_0##_p2), v_sum02);\
		v_sum01 = _mm256_add_pd(v_sum01, row_p1##_0);\
		v_sum11 = _mm256_add_pd(v_sum11, row_p1##_m1);\
		v_sum11 = _mm256_add_pd(v_sum11, row_p1##_p1);\
		v_m2_p1 = row_p1##_m2;\
		v_p2_p1 = row_p1##_p2;\
		v_sum02 = _mm256_add_pd(v_sum02, row_p2##_0);\
		v_sum22 = _mm256_add_pd(v_sum22, row_p2##_m2);\
		v_sum22 = _mm256_add_pd(v_sum22, row_p2##_p2);\
		v_sum12 = _mm256_add_pd(v_sum12, row_p2##_m1);\
		v_sum12 = _mm256_add_pd(v_sum12, row_p2##_p1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_m2_m1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_m2_p1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_p2_m1);\
		v_sum12 = _mm256_add_pd(v_sum12, v_p2_p1);\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc01, v_sum01));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc02, v_sum02));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc11, v_sum11));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc22, v_sum22));\
		vout = _mm256_add_pd(vout, _mm256_mul_pd(vc12, v_sum12));\
	}

#define Compute_25p_chunk(load_row, yy) {\
		int _chunk_y = (yy);\
		load_row(row0, _chunk_y - 2);\
		load_row(row1, _chunk_y - 1);\
		load_row(row2, _chunk_y    );\
		load_row(row3, _chunk_y + 1);\
		load_row(row4, _chunk_y + 2);\
		Compute_25p_window(row0, row1, row2, row3, row4);\
		Input_Output_1(out, vout, in);\
		vstore(BV[5][_chunk_y - YSTART][0], vout);\
		shift_row_window();\
		load_row(row4, _chunk_y + 3);\
		Compute_25p_window(row0, row1, row2, row3, row4);\
		Input_Output_2(out, vout, in);\
		vstore(BV[5][_chunk_y - YSTART + 1][0], vout);\
		shift_row_window();\
		load_row(row4, _chunk_y + 4);\
		Compute_25p_window(row0, row1, row2, row3, row4);\
		Input_Output_3(out, vout, in);\
		vstore(BV[5][_chunk_y - YSTART + 2][0], vout);\
		shift_row_window();\
		load_row(row4, _chunk_y + 5);\
		Compute_25p_window(row0, row1, row2, row3, row4);\
		Input_Output_4(out, vout, in);\
		vstore(BV[5][_chunk_y - YSTART + 3][0], vout);\
	}

#define Compute_25p_single(load_row, yy) {\
		int _single_y = (yy);\
		load_row(row0, _single_y - 2);\
		load_row(row1, _single_y - 1);\
		load_row(row2, _single_y    );\
		load_row(row3, _single_y + 1);\
		load_row(row4, _single_y + 2);\
		Compute_25p_window(row0, row1, row2, row3, row4);\
	}

#define store_time_vector(yy) {\
		_mm256_storeu_pd(tmp, vout);\
		B[(t+1)%2][x + STRIDE * 3][(yy)] = tmp[0];\
		B[t%2    ][x + STRIDE * 2][(yy)] = tmp[1];\
		B[(t+1)%2][x + STRIDE    ][(yy)] = tmp[2];\
		B[t%2    ][x             ][(yy)] = tmp[3];\
	}

void vectime(double* A, int NX, int NY, int T) {
	
	double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;
	double tmp[VECLEN];

	int tt, t =0, x, xx, y;
	int x_vector_end, y_vector_end;
	int bv_rows = (NY > 0) ? NY : 1;
	int last_transposed_y = YSTART - 1;

	vec v_center_0, v_center_1, v_center_2, v_center_3;
	vec row0_m2, row0_m1, row0_0, row0_p1, row0_p2;
	vec row1_m2, row1_m1, row1_0, row1_p1, row1_p2;
	vec row2_m2, row2_m1, row2_0, row2_p1, row2_p2;
	vec row3_m2, row3_m1, row3_0, row3_p1, row3_p2;
	vec row4_m2, row4_m1, row4_0, row4_p1, row4_p2;
	vec v_sum01, v_sum02, v_sum11, v_sum22, v_sum12;
	vec v_0_m1, v_m2_m1, v_p2_m1, v_m2_p1, v_p2_p1;
	vec in, out, vout;
	SET_COFF;

	if (NY >= VECLEN) {
		last_transposed_y = YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN - 1;
	}

	/* x 方向太短时没有完整的 temporal-vector 主体，直接退回 scalar。 */
	if (NX < STRIDE * VECLEN) {
		for ( t = 0; t < T; t++) {
			for ( x = XSTART; x < NX + XSTART; x++) {
				#pragma ivdep
				#pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}
		return;
	}

	double (* AV) [bv_rows][VECLEN] = (double(*)[bv_rows][VECLEN])alloc_extra_array(sizeof(double) * bv_rows * VECLEN * 6);

	double (* BV0) [VECLEN] = (double (*) [VECLEN]) AV;
	double (* BV1) [VECLEN] = (double (*) [VECLEN]) (AV + 1);
	double (* BV2) [VECLEN] = (double (*) [VECLEN]) (AV + 2);
	double (* BV3) [VECLEN] = (double (*) [VECLEN]) (AV + 3);
	double (* BV4) [VECLEN] = (double (*) [VECLEN]) (AV + 4);
	double (* BV5) [VECLEN] = (double (*) [VECLEN]) (AV + 5);

	double (* BV [6]) [VECLEN] = {BV0, BV1, BV2, BV3, BV4, BV5};
	double (* BV_tmp) [VECLEN];

	for ( tt = 0; tt <= T - VECLEN; tt += VECLEN) {

		for ( t = tt; t < tt + VECLEN - 1; t++) {		//head
			for ( x = XSTART; x < XSTART + STRIDE * (VECLEN - 1 - (t - tt)); x++) {
				#pragma ivdep
				#pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}
		t = tt;

		x_vector_end = NX + XSTART - STRIDE * VECLEN;
		y_vector_end = last_transposed_y - VECLEN + 1;
		BV[0] = BV0;
		BV[1] = BV1;
		BV[2] = BV2;
		BV[3] = BV3;
		BV[4] = BV4;
		BV[5] = BV5;

		/* 初始 5 个输入 slice 打包到 extra array：BV0..BV4 对应 x-2..x+2。 */
		for ( x = XSTART - XSLOPE; x <= XSTART + XSLOPE; x++) {
			for ( y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(v_center_0, B[t%2    ][x + STRIDE *3][y]);
				vload(v_center_1, B[(t+1)%2][x + STRIDE *2][y]);
				vload(v_center_2, B[t%2    ][x + STRIDE   ][y]);
				vload(v_center_3, B[(t+1)%2][x            ][y]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART) + 3][0], v_center_3);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART) + 2][0], v_center_2);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART) + 1][0], v_center_1);
				vstore(BV[x - (XSTART - XSLOPE)][(y - YSTART)    ][0], v_center_0);
			}
		}

		for ( x = XSTART; x <= x_vector_end; x++) {
			y = YSTART;

			/* y 低边界 chunk：显式 boundary loader，避免主路径分支。 */
			if (y <= y_vector_end) {
				vload(in, B[t%2][x + STRIDE * VECLEN][y]);
				Compute_25p_chunk(load_row_boundary, y);
				vstore(B[t%2][x][y], out);
				y += VECLEN;
			}

			/* y 主路径：所有 stencil 行都在 BV 中，只走无分支 BV loader，并复用 5 行寄存器窗口。 */
			for ( ; y + VECLEN - 1 + YSLOPE <= last_transposed_y; y += VECLEN) {
				vload(in, B[t%2][x + STRIDE * VECLEN][y]);	// the next x iter in vector
				Compute_25p_chunk(load_row_bv, y);
				vstore(B[t%2][x][y], out);
			}

			/* y 高边界 chunk：只处理剩余完整向量块。 */
			for ( ; y <= y_vector_end; y += VECLEN) {
				vload(in, B[t%2][x + STRIDE * VECLEN][y]);
				Compute_25p_chunk(load_row_boundary, y);
				vstore(B[t%2][x][y], out);
			}

			/* y 尾部不进入 BV 输出 slice，继续 direct gather/scatter。 */
			for ( y = last_transposed_y + 1; y < NY + YSTART; y++) {
				Compute_25p_single(load_row_boundary, y);
				store_time_vector(y);
			}

			BV_tmp = BV[0];
			BV[0] = BV[1];
			BV[1] = BV[2];
			BV[2] = BV[3];
			BV[3] = BV[4];
			BV[4] = BV[5];
			BV[5] = BV_tmp;
		}

		xx = x;

		/* x 主体结束后，把最后 5 个输入 slice 从 extra array 转回普通 B 布局。 */
		for ( x = xx - XSLOPE; x <= xx + XSLOPE; x++) {
			for ( y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
				vload(v_center_3, BV[x - (xx - XSLOPE)][(y - YSTART) + 3][0]);
				vload(v_center_2, BV[x - (xx - XSLOPE)][(y - YSTART) + 2][0]);
				vload(v_center_1, BV[x - (xx - XSLOPE)][(y - YSTART) + 1][0]);
				vload(v_center_0, BV[x - (xx - XSLOPE)][(y - YSTART)    ][0]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore(B[t%2    ][x + STRIDE *3][y], v_center_0);
				vstore(B[(t+1)%2][x + STRIDE *2][y], v_center_1);
				vstore(B[t%2    ][x + STRIDE   ][y], v_center_2);
				vstore(B[(t+1)%2][x            ][y], v_center_3);
			}
		}

		for ( t = tt; t < tt + VECLEN; t++) {		//tail
			for ( x = xx + STRIDE * (VECLEN - 1 - (t - tt)); x < NX + XSTART; x++) {
				#pragma ivdep
				#pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}

		/* extra array 不再改写左 halo，这里保留恢复动作，避免后续 tile 读到旧时间层差异。 */
		for ( x = XSTART - XSLOPE; x < XSTART; x++) {
			for ( y = 0; y < NY + YSTART * 2; y++) {
				B[1][x][y] = B[0][x][y];
			}
		}
	}

	for ( ; t < T; t++) {		//Extra points
		for ( x = XSTART; x < NX + XSTART; x++) {
			#pragma ivdep
			#pragma vector always
			for ( y = YSTART; y < NY + YSTART; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}

	free_extra_array(AV);
}

#undef store_time_vector
#undef Compute_25p_single
#undef Compute_25p_chunk
#undef Compute_25p_window
#undef shift_row_window
#undef copy_row
#undef load_row_boundary
#undef load_row_direct
#undef load_row_bv
