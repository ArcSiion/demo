#include "define.h"

/* 从 BV0..BV4 读取一个 y 行，并提前合成 c/h1/h2 三个 feature。 */
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

/* 从普通 B 数组直接 gather 一个 y 行，供 y 边界和 y tail 使用。 */
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

/* 使用 5 个 feature row 计算一个 25p 输出时间向量。 */
#define compute_25p_feature_window(row_m2, row_m1, row_0, row_p1, row_p2) {\
		/* C00: 中心点。 */\
		v_result = _mm256_mul_pd(vc00, row_0##_c);\
		/* C01: y=0 的横向一阶邻居 + x 中心列的 y 一阶邻居。 */\
		v_c01_sum = _mm256_add_pd(row_0##_h1, row_m1##_c);\
		v_c01_sum = _mm256_add_pd(v_c01_sum, row_p1##_c);\
		v_result = _mm256_fmadd_pd(vc01, v_c01_sum, v_result);\
		/* C02: y=0 的横向二阶邻居 + x 中心列的 y 二阶邻居。 */\
		v_c02_sum = _mm256_add_pd(row_0##_h2, row_m2##_c);\
		v_c02_sum = _mm256_add_pd(v_c02_sum, row_p2##_c);\
		v_result = _mm256_fmadd_pd(vc02, v_c02_sum, v_result);\
		/* C11/C22: 四个对角点按 feature row 直接相加。 */\
		v_c11_sum = _mm256_add_pd(row_m1##_h1, row_p1##_h1);\
		v_result = _mm256_fmadd_pd(vc11, v_c11_sum, v_result);\
		v_c22_sum = _mm256_add_pd(row_m2##_h2, row_p2##_h2);\
		v_result = _mm256_fmadd_pd(vc22, v_c22_sum, v_result);\
		/* C12: 一阶 x + 二阶 y，以及二阶 x + 一阶 y 的混合项。 */\
		v_c12_sum = _mm256_add_pd(row_m2##_h1, row_p2##_h1);\
		v_pair_tmp = _mm256_add_pd(row_m1##_h2, row_p1##_h2);\
		v_c12_sum = _mm256_add_pd(v_c12_sum, v_pair_tmp);\
		v_result = _mm256_fmadd_pd(vc12, v_c12_sum, v_result);\
	}

void vectime(double* A, int NX, int NY, int T) {
	
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;
	double lane_tmp[VECLEN];

	int tt, t =0, x, xx, y;
	int last_vector_x, last_full_chunk_y;
	int bv_y_rows = NY;
	int last_packed_y = YSTART + ((NY - VECLEN) / VECLEN) * VECLEN + VECLEN - 1;

	vec v_center_0, v_center_1, v_center_2, v_center_3;
	vec row_slot0_c, row_slot0_h1, row_slot0_h2;
	vec row_slot1_c, row_slot1_h1, row_slot1_h2;
	vec row_slot2_c, row_slot2_h1, row_slot2_h2;
	vec row_slot3_c, row_slot3_h1, row_slot3_h2;
	vec row_slot4_c, row_slot4_h1, row_slot4_h2;
	vec v_c01_sum, v_c02_sum, v_c11_sum, v_c22_sum, v_c12_sum;
	vec v_left_tmp, v_right_tmp, v_pair_tmp;
	vec in, out, v_result;
	SET_COFF;

	/* 当前版本假设 NX/NY 足够进入 temporal-vector 主体。 */
	double (* BV_buffer)[bv_y_rows][VECLEN] =
		(double (*)[bv_y_rows][VECLEN])alloc_extra_array(
			sizeof(double) * bv_y_rows * VECLEN * 6);

	double (* BV0) [VECLEN] = (double (*) [VECLEN]) BV_buffer;
	double (* BV1) [VECLEN] = (double (*) [VECLEN]) (BV_buffer + 1);
	double (* BV2) [VECLEN] = (double (*) [VECLEN]) (BV_buffer + 2);
	double (* BV3) [VECLEN] = (double (*) [VECLEN]) (BV_buffer + 3);
	double (* BV4) [VECLEN] = (double (*) [VECLEN]) (BV_buffer + 4);
	double (* BV5) [VECLEN] = (double (*) [VECLEN]) (BV_buffer + 5);

	double (* BV_rotate_tmp) [VECLEN];

	for ( tt = 0; tt <= T - VECLEN; tt += VECLEN) {

		/* 时间 tile 左侧依赖区仍用 scalar 预热。 */
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

		last_vector_x = NX + XSTART - STRIDE * VECLEN;
		last_full_chunk_y = last_packed_y - VECLEN + 1;
		BV0 = (double (*) [VECLEN]) BV_buffer;
		BV1 = (double (*) [VECLEN]) (BV_buffer + 1);
		BV2 = (double (*) [VECLEN]) (BV_buffer + 2);
		BV3 = (double (*) [VECLEN]) (BV_buffer + 3);
		BV4 = (double (*) [VECLEN]) (BV_buffer + 4);
		BV5 = (double (*) [VECLEN]) (BV_buffer + 5);

		/* 初始 5 个输入 slice 打包到 extra array：BV0..BV4 对应 x-2..x+2。 */
		{
			double (*BV_pack[5])[VECLEN] = {BV0, BV1, BV2, BV3, BV4};

			for (int bv_col = 0; bv_col < 5; bv_col++) {
				int pack_x = XSTART - XSLOPE + bv_col;

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
					vstore(BV_pack[bv_col][y - YSTART + 3][0], v_center_3);
					vstore(BV_pack[bv_col][y - YSTART + 2][0], v_center_2);
					vstore(BV_pack[bv_col][y - YSTART + 1][0], v_center_1);
					vstore(BV_pack[bv_col][y - YSTART][0], v_center_0);
				}
			}
		}

		for ( x = XSTART; x <= last_vector_x; x++) {
			y = YSTART;

			/* y 低边界 chunk：显式 boundary loader，避免主路径分支。 */
			if (y <= last_full_chunk_y) {
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
				load_feature_row_direct(row_slot0, y - 2);
				load_feature_row_direct(row_slot1, y - 1);
				load_feature_row_from_bv(row_slot2, y    );
				load_feature_row_from_bv(row_slot3, y + 1);
				load_feature_row_from_bv(row_slot4, y + 2);

				compute_25p_feature_window(row_slot0, row_slot1,
										   row_slot2, row_slot3, row_slot4);
				Input_Output_1(out, v_result, in);
				vstore(BV5[y - YSTART][0], v_result);

				load_feature_row_from_bv(row_slot0, y + 3);
				compute_25p_feature_window(row_slot1, row_slot2,
										   row_slot3, row_slot4, row_slot0);
				Input_Output_2(out, v_result, in);
				vstore(BV5[y - YSTART + 1][0], v_result);

				load_feature_row_from_bv(row_slot1, y + 4);
				compute_25p_feature_window(row_slot2, row_slot3,
										   row_slot4, row_slot0, row_slot1);
				Input_Output_3(out, v_result, in);
				vstore(BV5[y - YSTART + 2][0], v_result);

				load_feature_row_from_bv(row_slot2, y + 5);
				compute_25p_feature_window(row_slot3, row_slot4,
										   row_slot0, row_slot1, row_slot2);
				Input_Output_4(out, v_result, in);
				vstore(BV5[y - YSTART + 3][0], v_result);
				_mm256_storeu_pd(&B[t%2][x][y], out);
				y += VECLEN;
			}

			/* y 主路径：所有 stencil 行都在 BV 中，只走无分支 BV loader，并复用 5 行寄存器窗口。 */
			for ( ; y + VECLEN - 1 + YSLOPE <= last_packed_y; y += VECLEN) {
				/* in 是下一轮 x 迭代需要拼入 Input_Output 的右侧时间向量。 */
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
				load_feature_row_from_bv(row_slot0, y - 2);
				load_feature_row_from_bv(row_slot1, y - 1);
				load_feature_row_from_bv(row_slot2, y    );
				load_feature_row_from_bv(row_slot3, y + 1);
				load_feature_row_from_bv(row_slot4, y + 2);

				compute_25p_feature_window(row_slot0, row_slot1,
										   row_slot2, row_slot3, row_slot4);
				Input_Output_1(out, v_result, in);
				vstore(BV5[y - YSTART][0], v_result);

				load_feature_row_from_bv(row_slot0, y + 3);
				compute_25p_feature_window(row_slot1, row_slot2,
										   row_slot3, row_slot4, row_slot0);
				Input_Output_2(out, v_result, in);
				vstore(BV5[y - YSTART + 1][0], v_result);

				load_feature_row_from_bv(row_slot1, y + 4);
				compute_25p_feature_window(row_slot2, row_slot3,
										   row_slot4, row_slot0, row_slot1);
				Input_Output_3(out, v_result, in);
				vstore(BV5[y - YSTART + 2][0], v_result);

				load_feature_row_from_bv(row_slot2, y + 5);
				compute_25p_feature_window(row_slot3, row_slot4,
										   row_slot0, row_slot1, row_slot2);
				Input_Output_4(out, v_result, in);
				vstore(BV5[y - YSTART + 3][0], v_result);
				_mm256_storeu_pd(&B[t%2][x][y], out);
			}

			/* y 高边界 chunk：只处理剩余完整向量块。 */
			for ( ; y <= last_full_chunk_y; y += VECLEN) {
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
				load_feature_row_from_bv(row_slot0, y - 2);
				load_feature_row_from_bv(row_slot1, y - 1);
				load_feature_row_from_bv(row_slot2, y    );
				load_feature_row_from_bv(row_slot3, y + 1);
				load_feature_row_from_bv(row_slot4, y + 2);

				compute_25p_feature_window(row_slot0, row_slot1,
										   row_slot2, row_slot3, row_slot4);
				Input_Output_1(out, v_result, in);
				vstore(BV5[y - YSTART][0], v_result);

				load_feature_row_from_bv(row_slot0, y + 3);
				compute_25p_feature_window(row_slot1, row_slot2,
										   row_slot3, row_slot4, row_slot0);
				Input_Output_2(out, v_result, in);
				vstore(BV5[y - YSTART + 1][0], v_result);

				load_feature_row_direct(row_slot1, y + 4);
				compute_25p_feature_window(row_slot2, row_slot3,
										   row_slot4, row_slot0, row_slot1);
				Input_Output_3(out, v_result, in);
				vstore(BV5[y - YSTART + 2][0], v_result);

				load_feature_row_direct(row_slot2, y + 5);
				compute_25p_feature_window(row_slot3, row_slot4,
										   row_slot0, row_slot1, row_slot2);
				Input_Output_4(out, v_result, in);
				vstore(BV5[y - YSTART + 3][0], v_result);
				_mm256_storeu_pd(&B[t%2][x][y], out);
			}

			/* y 尾部不进入 BV 输出 slice，继续 direct gather/scatter。 */
			for ( y = last_packed_y + 1; y < NY + YSTART; y++) {
				if (y - 2 <= last_packed_y) {
					load_feature_row_from_bv(row_slot0, y - 2);
				} else {
					load_feature_row_direct(row_slot0, y - 2);
				}

				if (y - 1 <= last_packed_y) {
					load_feature_row_from_bv(row_slot1, y - 1);
				} else {
					load_feature_row_direct(row_slot1, y - 1);
				}

				load_feature_row_direct(row_slot2, y    );
				load_feature_row_direct(row_slot3, y + 1);
				load_feature_row_direct(row_slot4, y + 2);

				compute_25p_feature_window(
					row_slot0, row_slot1, row_slot2,
					row_slot3, row_slot4);

				_mm256_storeu_pd(lane_tmp, v_result);
				B[(t+1)%2][x + STRIDE * 3][y] = lane_tmp[0];
				B[t%2    ][x + STRIDE * 2][y] = lane_tmp[1];
				B[(t+1)%2][x + STRIDE    ][y] = lane_tmp[2];
				B[t%2    ][x             ][y] = lane_tmp[3];
			}

			/* BV0 是当前窗口最左列；本轮输出写在 BV5，轮转后成为新的最右列。 */
			BV_rotate_tmp = BV0;
			BV0 = BV1;
			BV1 = BV2;
			BV2 = BV3;
			BV3 = BV4;
			BV4 = BV5;
			BV5 = BV_rotate_tmp;
		}

		xx = x;

		/* x 主体结束后，把最后 5 个输入 slice 转回普通 B 布局。 */
		{
			double (*BV_unpack[5])[VECLEN] = {BV0, BV1, BV2, BV3, BV4};

			for (int bv_col = 0; bv_col < 5; bv_col++) {
				int unpack_x = xx - XSLOPE + bv_col;

				for (y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {
					vload(v_center_3, BV_unpack[bv_col][y - YSTART + 3][0]);
					vload(v_center_2, BV_unpack[bv_col][y - YSTART + 2][0]);
					vload(v_center_1, BV_unpack[bv_col][y - YSTART + 1][0]);
					vload(v_center_0, BV_unpack[bv_col][y - YSTART][0]);
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

		/* 时间 tile 右侧依赖区回到 scalar 收尾。 */
		for ( t = tt; t < tt + VECLEN; t++) {
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

	/* T 不能被 VECLEN 整除时，剩余时间步用 scalar 完成。 */
	for ( ; t < T; t++) {
		for ( x = XSTART; x < NX + XSTART; x++) {
			#pragma ivdep
			#pragma vector always
			for ( y = YSTART; y < NY + YSTART; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}

	free_extra_array(BV_buffer);
}

#undef compute_25p_feature_window
#undef load_feature_row_direct
#undef load_feature_row_from_bv
