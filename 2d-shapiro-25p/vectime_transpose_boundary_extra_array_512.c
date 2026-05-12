#include "define_512.h"

void vectime_transpose_boundary_extra_array_512(double* A, int NX, int NY, int T) {
	
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;
	double tmp[VVECLEN];

	int tt, t = 0, x, xx, y;

	// 512 版需要完整的 8-lane x/y 主体；尺寸不足时直接退回 scalar。
	if (NX < STRIDE * VVECLEN || NY < VVECLEN) {
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

	vec_512 v_center_0, v_center_1, v_center_2, v_center_3;
	vec_512 v_center_4, v_center_5, v_center_6, v_center_7;
	vec_512 r0_c, r0_d1, r0_d2;
	vec_512 r1_c, r1_d1, r1_d2;
	vec_512 r2_c, r2_d1, r2_d2;
	vec_512 r3_c, r3_d1, r3_d2;
	vec_512 r4_c, r4_d1, r4_d2;
	vec_512 sum_c01, sum_c02, sum_c11, sum_c22, sum_c12;
	vec_512 tmp_left, tmp_right, sum_d2_pair;
	vec_512 in, out, v_result;
	SET_COFF_512;

	double (* AV)[NY][VVECLEN] = (double (*)[NY][VVECLEN])
		alloc_extra_array_512(sizeof(double) * NY * VVECLEN * 6);

	double (* BV0)[VVECLEN] = (double (*)[VVECLEN]) AV;
	double (* BV1)[VVECLEN] = (double (*)[VVECLEN]) (AV + 1);
	double (* BV2)[VVECLEN] = (double (*)[VVECLEN]) (AV + 2);
	double (* BV3)[VVECLEN] = (double (*)[VVECLEN]) (AV + 3);
	double (* BV4)[VVECLEN] = (double (*)[VVECLEN]) (AV + 4);
	double (* BV5)[VVECLEN] = (double (*)[VVECLEN]) (AV + 5);

	double (* BVtmp)[VVECLEN];

	for ( tt = 0; tt <= T - VVECLEN; tt += VVECLEN) {

		// 时间 tile 左侧依赖区仍用 scalar 预热。
		for ( t = tt; t < tt + VVECLEN - 1; t++) {
			for ( x = XSTART; x < XSTART + STRIDE * (VVECLEN - 1 - (t - tt)); x++) {
				#pragma ivdep
				#pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}
		t = tt;

		BV0 = (double (*)[VVECLEN]) AV;
		BV1 = (double (*)[VVECLEN]) (AV + 1);
		BV2 = (double (*)[VVECLEN]) (AV + 2);
		BV3 = (double (*)[VVECLEN]) (AV + 3);
		BV4 = (double (*)[VVECLEN]) (AV + 4);
		BV5 = (double (*)[VVECLEN]) (AV + 5);

		// 初始 5 个输入 slice 打包到 extra array：BV0..BV4 对应 x-2..x+2。
		{
			double (*Btmp[5])[VVECLEN] = {BV0, BV1, BV2, BV3, BV4};

			for (int i = 0; i < 5; i++) {
				int pack_x = XSTART - XSLOPE + i;

				for (y = YSTART; y <= NY + YSTART - VVECLEN; y += VVECLEN) {
					v_center_0 = _mm512_loadu_pd(
						&B[t%2    ][pack_x + STRIDE * 7][y]);
					v_center_1 = _mm512_loadu_pd(
						&B[(t+1)%2][pack_x + STRIDE * 6][y]);
					v_center_2 = _mm512_loadu_pd(
						&B[t%2    ][pack_x + STRIDE * 5][y]);
					v_center_3 = _mm512_loadu_pd(
						&B[(t+1)%2][pack_x + STRIDE * 4][y]);
					v_center_4 = _mm512_loadu_pd(
						&B[t%2    ][pack_x + STRIDE * 3][y]);
					v_center_5 = _mm512_loadu_pd(
						&B[(t+1)%2][pack_x + STRIDE * 2][y]);
					v_center_6 = _mm512_loadu_pd(
						&B[t%2    ][pack_x + STRIDE    ][y]);
					v_center_7 = _mm512_loadu_pd(
						&B[(t+1)%2][pack_x             ][y]);

					transpose_512(v_center_0, v_center_1,
								  v_center_2, v_center_3,
								  v_center_4, v_center_5,
								  v_center_6, v_center_7, in, out);

					vstore_512(Btmp[i][y - YSTART][0], v_center_0);
					vstore_512(Btmp[i][y - YSTART + 1][0], v_center_1);
					vstore_512(Btmp[i][y - YSTART + 2][0], v_center_2);
					vstore_512(Btmp[i][y - YSTART + 3][0], v_center_3);
					vstore_512(Btmp[i][y - YSTART + 4][0], v_center_4);
					vstore_512(Btmp[i][y - YSTART + 5][0], v_center_5);
					vstore_512(Btmp[i][y - YSTART + 6][0], v_center_6);
					vstore_512(Btmp[i][y - YSTART + 7][0], v_center_7);
				}
			}
		}

		// x 主体最后一个起点：右侧保留 STRIDE * VVECLEN 列给 Input_Output。
		for ( x = XSTART; x <= NX + XSTART - STRIDE * VVECLEN; x++) {
			y = YSTART;

			// y 低边界 chunk：前两行直接从 B 读取，其余尽量从 BV 读取。
			// 最后一个完整 y chunk 起点：保证 y..y+VVECLEN-1 都在打包区域。
			if (y <= YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN) {
				in = _mm512_loadu_pd(&B[t%2][x + STRIDE * VVECLEN][y]);
				load_sum_direct_512(r0_c, r0_d1, r0_d2, y - 2);
				load_sum_direct_512(r1_c, r1_d1, r1_d2, y - 1);
				load_sum_from_bv_512(r2_c, r2_d1, r2_d2, y    );
				load_sum_from_bv_512(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_from_bv_512(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_1_512(out, v_result, in);
				vstore_512(BV5[y - YSTART][0], v_result);

				load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y + 3);
				Compute_1vector_25p_512(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_2_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 1][0], v_result);

				load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y + 4);
				Compute_1vector_25p_512(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_3_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 2][0], v_result);

				load_sum_from_bv_512(r2_c, r2_d1, r2_d2, y + 5);
				Compute_1vector_25p_512(
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2);
				Input_Output_4_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 3][0], v_result);

				load_sum_from_bv_512(r3_c, r3_d1, r3_d2, y + 6);
				Compute_1vector_25p_512(
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2);
				Input_Output_5_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 4][0], v_result);

				load_sum_from_bv_512(r4_c, r4_d1, r4_d2, y + 7);
				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_6_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 5][0], v_result);

				if (y + 8 <=
					YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN + VVECLEN - 1) {
					load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y + 8);
				} else {
					load_sum_direct_512(r0_c, r0_d1, r0_d2, y + 8);
				}
				Compute_1vector_25p_512(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_7_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 6][0], v_result);

				if (y + 9 <=
					YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN + VVECLEN - 1) {
					load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y + 9);
				} else {
					load_sum_direct_512(r1_c, r1_d1, r1_d2, y + 9);
				}
				Compute_1vector_25p_512(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_8_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 7][0], v_result);
				_mm512_storeu_pd(&B[t%2][x][y], out);
				y += VVECLEN;
			}

			// y 主路径：所有 stencil 行都在 BV 中，只走无分支 BV loader。
			// 最后一个打包 y：BV 只保存完整 VVECLEN 分块覆盖到的行。
			for ( ; y + VVECLEN - 1 + YSLOPE <=
					YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN + VVECLEN - 1;
				  y += VVECLEN) {
				// in 是下一轮 x 迭代需要拼入 Input_Output 的右侧时间向量。
				in = _mm512_loadu_pd(&B[t%2][x + STRIDE * VVECLEN][y]);
				load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y - 2);
				load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y - 1);
				load_sum_from_bv_512(r2_c, r2_d1, r2_d2, y    );
				load_sum_from_bv_512(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_from_bv_512(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_1_512(out, v_result, in);
				vstore_512(BV5[y - YSTART][0], v_result);

				load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y + 3);
				Compute_1vector_25p_512(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_2_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 1][0], v_result);

				load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y + 4);
				Compute_1vector_25p_512(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_3_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 2][0], v_result);

				load_sum_from_bv_512(r2_c, r2_d1, r2_d2, y + 5);
				Compute_1vector_25p_512(
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2);
				Input_Output_4_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 3][0], v_result);

				load_sum_from_bv_512(r3_c, r3_d1, r3_d2, y + 6);
				Compute_1vector_25p_512(
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2);
				Input_Output_5_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 4][0], v_result);

				load_sum_from_bv_512(r4_c, r4_d1, r4_d2, y + 7);
				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_6_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 5][0], v_result);

				load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y + 8);
				Compute_1vector_25p_512(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_7_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 6][0], v_result);

				load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y + 9);
				Compute_1vector_25p_512(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_8_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 7][0], v_result);
				_mm512_storeu_pd(&B[t%2][x][y], out);
			}

			// y 高边界 chunk：最后两行超出 BV 打包区，直接从 B 读取。
			for ( ; y <= YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN;
				  y += VVECLEN) {
				in = _mm512_loadu_pd(&B[t%2][x + STRIDE * VVECLEN][y]);
				load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y - 2);
				load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y - 1);
				load_sum_from_bv_512(r2_c, r2_d1, r2_d2, y    );
				load_sum_from_bv_512(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_from_bv_512(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_1_512(out, v_result, in);
				vstore_512(BV5[y - YSTART][0], v_result);

				load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y + 3);
				Compute_1vector_25p_512(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_2_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 1][0], v_result);

				load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y + 4);
				Compute_1vector_25p_512(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_3_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 2][0], v_result);

				load_sum_from_bv_512(r2_c, r2_d1, r2_d2, y + 5);
				Compute_1vector_25p_512(
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2);
				Input_Output_4_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 3][0], v_result);

				load_sum_from_bv_512(r3_c, r3_d1, r3_d2, y + 6);
				Compute_1vector_25p_512(
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2);
				Input_Output_5_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 4][0], v_result);

				load_sum_from_bv_512(r4_c, r4_d1, r4_d2, y + 7);
				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_6_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 5][0], v_result);

				load_sum_direct_512(r0_c, r0_d1, r0_d2, y + 8);
				Compute_1vector_25p_512(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_7_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 6][0], v_result);

				load_sum_direct_512(r1_c, r1_d1, r1_d2, y + 9);
				Compute_1vector_25p_512(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_8_512(out, v_result, in);
				vstore_512(BV5[y - YSTART + 7][0], v_result);
				_mm512_storeu_pd(&B[t%2][x][y], out);
			}

			// y 尾部不进入 BV 输出 slice，继续 direct gather/scatter。
			for ( y = YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN + VVECLEN;
				  y < NY + YSTART; y++) {
				if (y - 2 <=
					YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN + VVECLEN - 1) {
					load_sum_from_bv_512(r0_c, r0_d1, r0_d2, y - 2);
				} else {
					load_sum_direct_512(r0_c, r0_d1, r0_d2, y - 2);
				}

				if (y - 1 <=
					YSTART + ((NY - VVECLEN) / VVECLEN) * VVECLEN + VVECLEN - 1) {
					load_sum_from_bv_512(r1_c, r1_d1, r1_d2, y - 1);
				} else {
					load_sum_direct_512(r1_c, r1_d1, r1_d2, y - 1);
				}

				load_sum_direct_512(r2_c, r2_d1, r2_d2, y    );
				load_sum_direct_512(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_direct_512(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p_512(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);

				_mm512_storeu_pd(tmp, v_result);
				B[(t+1)%2][x + STRIDE * 7][y] = tmp[0];
				B[t%2    ][x + STRIDE * 6][y] = tmp[1];
				B[(t+1)%2][x + STRIDE * 5][y] = tmp[2];
				B[t%2    ][x + STRIDE * 4][y] = tmp[3];
				B[(t+1)%2][x + STRIDE * 3][y] = tmp[4];
				B[t%2    ][x + STRIDE * 2][y] = tmp[5];
				B[(t+1)%2][x + STRIDE    ][y] = tmp[6];
				B[t%2    ][x             ][y] = tmp[7];
			}

			// BV0 是当前窗口最左列；本轮输出写在 BV5，轮转后成为新的最右列。
			BVtmp = BV0;
			BV0 = BV1;
			BV1 = BV2;
			BV2 = BV3;
			BV3 = BV4;
			BV4 = BV5;
			BV5 = BVtmp;
		}

		xx = x;

		// x 主体结束后，把最后 5 个输入 slice 转回普通 B 布局。
		{
			double (*Btmp[5])[VVECLEN] = {BV0, BV1, BV2, BV3, BV4};

			for (int i = 0; i < 5; i++) {
				int unpack_x = xx - XSLOPE + i;

				for (y = YSTART; y <= NY + YSTART - VVECLEN; y += VVECLEN) {
					vload_512(v_center_0, Btmp[i][y - YSTART][0]);
					vload_512(v_center_1, Btmp[i][y - YSTART + 1][0]);
					vload_512(v_center_2, Btmp[i][y - YSTART + 2][0]);
					vload_512(v_center_3, Btmp[i][y - YSTART + 3][0]);
					vload_512(v_center_4, Btmp[i][y - YSTART + 4][0]);
					vload_512(v_center_5, Btmp[i][y - YSTART + 5][0]);
					vload_512(v_center_6, Btmp[i][y - YSTART + 6][0]);
					vload_512(v_center_7, Btmp[i][y - YSTART + 7][0]);

					transpose_512(v_center_0, v_center_1,
								  v_center_2, v_center_3,
								  v_center_4, v_center_5,
								  v_center_6, v_center_7, in, out);

					_mm512_storeu_pd(&B[t%2][unpack_x + STRIDE * 7][y],
									  v_center_0);
					_mm512_storeu_pd(&B[(t+1)%2][unpack_x + STRIDE * 6][y],
									  v_center_1);
					_mm512_storeu_pd(&B[t%2][unpack_x + STRIDE * 5][y],
									  v_center_2);
					_mm512_storeu_pd(&B[(t+1)%2][unpack_x + STRIDE * 4][y],
									  v_center_3);
					_mm512_storeu_pd(&B[t%2][unpack_x + STRIDE * 3][y],
									  v_center_4);
					_mm512_storeu_pd(&B[(t+1)%2][unpack_x + STRIDE * 2][y],
									  v_center_5);
					_mm512_storeu_pd(&B[t%2][unpack_x + STRIDE][y],
									  v_center_6);
					_mm512_storeu_pd(&B[(t+1)%2][unpack_x][y],
									  v_center_7);
				}
			}
		}

		// 时间 tile 右侧依赖区回到 scalar 收尾。
		for ( t = tt; t < tt + VVECLEN; t++) {
			for ( x = xx + STRIDE * (VVECLEN - 1 - (t - tt)); x < NX + XSTART; x++) {
				#pragma ivdep
				#pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}

		// extra array 不再改写左 halo，这里保留恢复动作，避免后续 tile 读到旧时间层差异。
		for ( x = XSTART - XSLOPE; x < XSTART; x++) {
			for ( y = 0; y < NY + YSTART * 2; y++) {
				B[1][x][y] = B[0][x][y];
			}
		}
	}

	// T 不能被 VVECLEN 整除时，剩余时间步用 scalar 完成。
	for ( ; t < T; t++) {
		for ( x = XSTART; x < NX + XSTART; x++) {
			#pragma ivdep
			#pragma vector always
			for ( y = YSTART; y < NY + YSTART; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}

	free_extra_array_512(AV);
}
