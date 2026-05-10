#include "define.h"

void vectime_transpose_boundary_extra_array(double* A, int NX, int NY, int T) {
	
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;
	double tmp[VECLEN];

	int tt, t =0, x, xx, y;

	vec v_center_0, v_center_1, v_center_2, v_center_3;
	vec r0_c, r0_d1, r0_d2;
	vec r1_c, r1_d1, r1_d2;
	vec r2_c, r2_d1, r2_d2;
	vec r3_c, r3_d1, r3_d2;
	vec r4_c, r4_d1, r4_d2;
	vec sum_c01, sum_c02, sum_c11, sum_c22, sum_c12;
	vec tmp_left, tmp_right, sum_d2_pair;
	vec in, out, v_result;
	SET_COFF;

	// 当前版本假设 NX/NY 足够进入 temporal-vector 主体。
	double (* AV)[NY][VECLEN] = (double (*)[NY][VECLEN])
		alloc_extra_array(sizeof(double) * NY * VECLEN * 6);

	double (* BV0)[VECLEN] = (double (*)[VECLEN]) AV;
	double (* BV1)[VECLEN] = (double (*)[VECLEN]) (AV + 1);
	double (* BV2)[VECLEN] = (double (*)[VECLEN]) (AV + 2);
	double (* BV3)[VECLEN] = (double (*)[VECLEN]) (AV + 3);
	double (* BV4)[VECLEN] = (double (*)[VECLEN]) (AV + 4);
	double (* BV5)[VECLEN] = (double (*)[VECLEN]) (AV + 5);

	double (* BVtmp)[VECLEN];

	for ( tt = 0; tt <= T - VECLEN; tt += VECLEN) {

		// 时间 tile 左侧依赖区仍用 scalar 预热。
		for ( t = tt; t < tt + VECLEN - 1; t++) {
			for ( x = XSTART; x < XSTART + STRIDE * (VECLEN - 1 - (t - tt)); x++) {
				#pragma ivdep
				#pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}
		t = tt;

		BV0 = (double (*)[VECLEN]) AV;
		BV1 = (double (*)[VECLEN]) (AV + 1);
		BV2 = (double (*)[VECLEN]) (AV + 2);
		BV3 = (double (*)[VECLEN]) (AV + 3);
		BV4 = (double (*)[VECLEN]) (AV + 4);
		BV5 = (double (*)[VECLEN]) (AV + 5);

		// 初始 5 个输入 slice 打包到 extra array：BV0..BV4 对应 x-2..x+2。
		{
			double (*Btmp[5])[VECLEN] = {BV0, BV1, BV2, BV3, BV4};

			for (int i = 0; i < 5; i++) {
				int pack_x = XSTART - XSLOPE + i;

				for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
					v_center_0 = _mm256_loadu_pd(
						&B[t%2    ][pack_x + STRIDE * 3][y]);
					v_center_1 = _mm256_loadu_pd(
						&B[(t+1)%2][pack_x + STRIDE * 2][y]);
					v_center_2 = _mm256_loadu_pd(
						&B[t%2    ][pack_x + STRIDE    ][y]);
					v_center_3 = _mm256_loadu_pd(
						&B[(t+1)%2][pack_x             ][y]);
					transpose(v_center_0, v_center_1,
							  v_center_2, v_center_3, in, out);
					vstore(Btmp[i][y - YSTART + 3][0], v_center_3);
					vstore(Btmp[i][y - YSTART + 2][0], v_center_2);
					vstore(Btmp[i][y - YSTART + 1][0], v_center_1);
					vstore(Btmp[i][y - YSTART][0], v_center_0);
				}
			}
		}

		// x 主体最后一个起点：右侧保留 STRIDE * VECLEN 列给 Input_Output。
		for ( x = XSTART; x <= NX + XSTART - STRIDE * VECLEN; x++) {
			y = YSTART;

			// y 低边界 chunk：显式 boundary loader，避免主路径分支。
			// 最后一个完整 y chunk 起点：保证 y..y+VECLEN-1 都在打包区域。
			if (y <= YSTART + ((NY - VECLEN) / VECLEN) * VECLEN) {
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
				load_sum_direct(r0_c, r0_d1, r0_d2, y - 2);
				load_sum_direct(r1_c, r1_d1, r1_d2, y - 1);
				load_sum_from_bv(r2_c, r2_d1, r2_d2, y    );
				load_sum_from_bv(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_from_bv(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_1(out, v_result, in);
				vstore(BV5[y - YSTART][0], v_result);

				load_sum_from_bv(r0_c, r0_d1, r0_d2, y + 3);
				Compute_1vector_25p(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_2(out, v_result, in);
				vstore(BV5[y - YSTART + 1][0], v_result);

				load_sum_from_bv(r1_c, r1_d1, r1_d2, y + 4);
				Compute_1vector_25p(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_3(out, v_result, in);
				vstore(BV5[y - YSTART + 2][0], v_result);

				load_sum_from_bv(r2_c, r2_d1, r2_d2, y + 5);
				Compute_1vector_25p(
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2);
				Input_Output_4(out, v_result, in);
				vstore(BV5[y - YSTART + 3][0], v_result);
				_mm256_storeu_pd(&B[t%2][x][y], out);
				y += VECLEN;
			}

			// y 主路径：所有 stencil 行都在 BV 中，只走无分支 BV loader，并复用 5 行寄存器窗口。
			// 最后一个打包 y：BV 只保存完整 VECLEN 分块覆盖到的行。
			for ( ; y + VECLEN - 1 + YSLOPE <=
					YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN - 1;
				  y += VECLEN) {
				// in 是下一轮 x 迭代需要拼入 Input_Output 的右侧时间向量。
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
				load_sum_from_bv(r0_c, r0_d1, r0_d2, y - 2);
				load_sum_from_bv(r1_c, r1_d1, r1_d2, y - 1);
				load_sum_from_bv(r2_c, r2_d1, r2_d2, y    );
				load_sum_from_bv(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_from_bv(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_1(out, v_result, in);
				vstore(BV5[y - YSTART][0], v_result);

				load_sum_from_bv(r0_c, r0_d1, r0_d2, y + 3);
				Compute_1vector_25p(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_2(out, v_result, in);
				vstore(BV5[y - YSTART + 1][0], v_result);

				load_sum_from_bv(r1_c, r1_d1, r1_d2, y + 4);
				Compute_1vector_25p(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_3(out, v_result, in);
				vstore(BV5[y - YSTART + 2][0], v_result);

				load_sum_from_bv(r2_c, r2_d1, r2_d2, y + 5);
				Compute_1vector_25p(
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2);
				Input_Output_4(out, v_result, in);
				vstore(BV5[y - YSTART + 3][0], v_result);
				_mm256_storeu_pd(&B[t%2][x][y], out);
			}

			// y 高边界 chunk：只处理剩余完整向量块。
			for ( ; y <= YSTART + ((NY - VECLEN) / VECLEN) * VECLEN;
				  y += VECLEN) {
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
				load_sum_from_bv(r0_c, r0_d1, r0_d2, y - 2);
				load_sum_from_bv(r1_c, r1_d1, r1_d2, y - 1);
				load_sum_from_bv(r2_c, r2_d1, r2_d2, y    );
				load_sum_from_bv(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_from_bv(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);
				Input_Output_1(out, v_result, in);
				vstore(BV5[y - YSTART][0], v_result);

				load_sum_from_bv(r0_c, r0_d1, r0_d2, y + 3);
				Compute_1vector_25p(
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2);
				Input_Output_2(out, v_result, in);
				vstore(BV5[y - YSTART + 1][0], v_result);

				load_sum_direct(r1_c, r1_d1, r1_d2, y + 4);
				Compute_1vector_25p(
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2);
				Input_Output_3(out, v_result, in);
				vstore(BV5[y - YSTART + 2][0], v_result);

				load_sum_direct(r2_c, r2_d1, r2_d2, y + 5);
				Compute_1vector_25p(
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2,
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2);
				Input_Output_4(out, v_result, in);
				vstore(BV5[y - YSTART + 3][0], v_result);
				_mm256_storeu_pd(&B[t%2][x][y], out);
			}

			// y 尾部不进入 BV 输出 slice，继续 direct gather/scatter。
			for ( y = YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN;
				  y < NY + YSTART; y++) {
				if (y - 2 <=
					YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN - 1) {
					load_sum_from_bv(r0_c, r0_d1, r0_d2, y - 2);
				} else {
					load_sum_direct(r0_c, r0_d1, r0_d2, y - 2);
				}

				if (y - 1 <=
					YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN - 1) {
					load_sum_from_bv(r1_c, r1_d1, r1_d2, y - 1);
				} else {
					load_sum_direct(r1_c, r1_d1, r1_d2, y - 1);
				}

				load_sum_direct(r2_c, r2_d1, r2_d2, y    );
				load_sum_direct(r3_c, r3_d1, r3_d2, y + 1);
				load_sum_direct(r4_c, r4_d1, r4_d2, y + 2);

				Compute_1vector_25p(
					r0_c, r0_d1, r0_d2,
					r1_c, r1_d1, r1_d2,
					r2_c, r2_d1, r2_d2,
					r3_c, r3_d1, r3_d2,
					r4_c, r4_d1, r4_d2);

				_mm256_storeu_pd(tmp, v_result);
				B[(t+1)%2][x + STRIDE * 3][y] = tmp[0];
				B[t%2    ][x + STRIDE * 2][y] = tmp[1];
				B[(t+1)%2][x + STRIDE    ][y] = tmp[2];
				B[t%2    ][x             ][y] = tmp[3];
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
			double (*Btmp[5])[VECLEN] = {BV0, BV1, BV2, BV3, BV4};

			for (int i = 0; i < 5; i++) {
				int unpack_x = xx - XSLOPE + i;

				for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
					vload(v_center_3, Btmp[i][y - YSTART + 3][0]);
					vload(v_center_2, Btmp[i][y - YSTART + 2][0]);
					vload(v_center_1, Btmp[i][y - YSTART + 1][0]);
					vload(v_center_0, Btmp[i][y - YSTART][0]);
					transpose(v_center_0, v_center_1,
							  v_center_2, v_center_3, in, out);
					_mm256_storeu_pd(&B[t%2][unpack_x + STRIDE * 3][y],
									  v_center_0);
					_mm256_storeu_pd(&B[(t+1)%2][unpack_x + STRIDE * 2][y],
									  v_center_1);
					_mm256_storeu_pd(&B[t%2][unpack_x + STRIDE][y],
									  v_center_2);
					_mm256_storeu_pd(&B[(t+1)%2][unpack_x][y],
									  v_center_3);
				}
			}
		}

		// 时间 tile 右侧依赖区回到 scalar 收尾。
		for ( t = tt; t < tt + VECLEN; t++) {
			for ( x = xx + STRIDE * (VECLEN - 1 - (t - tt)); x < NX + XSTART; x++) {
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

	// T 不能被 VECLEN 整除时，剩余时间步用 scalar 完成。
	for ( ; t < T; t++) {
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
