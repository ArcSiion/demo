#include "define.h"

/* 从 BV extra array 读取一个 32 字节对齐的时间向量。 */
#define load_bv_aligned(a,b) a=_mm256_load_pd((&b))

/* 向 BV extra array 写入一个 32 字节对齐的时间向量。 */
#define store_bv_aligned(a,b) _mm256_store_pd((&a),b)

/* 从 BV0..BV4 读取一个 y 行，并提前合成 c/h1/h2 三个 feature。 */
#define load_feature_row_from_bv(row, yy) {\
		int _bv_y = (yy);\
		load_bv_aligned(row##_c, BV2[_bv_y - YSTART][0]);\
		load_bv_aligned(v_left_tmp, BV1[_bv_y - YSTART][0]);\
		load_bv_aligned(v_right_tmp, BV3[_bv_y - YSTART][0]);\
		row##_h1 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
		load_bv_aligned(v_left_tmp, BV0[_bv_y - YSTART][0]);\
		load_bv_aligned(v_right_tmp, BV4[_bv_y - YSTART][0]);\
		row##_h2 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
	}

/* 从普通 B 数组直接 gather 一个 y 行，供 y 边界和 y tail 使用。 */
#define load_feature_row_direct(row, yy) {\
		int _direct_y = (yy);\
		vloadset(row##_c,	B[(t+1)%2][x                 ][_direct_y],\
							B[t%2    ][x     + STRIDE    ][_direct_y],\
							B[(t+1)%2][x     + STRIDE * 2][_direct_y],\
							B[t%2    ][x     + STRIDE * 3][_direct_y]);\
		vloadset(v_left_tmp,	B[(t+1)%2][x - 1             ][_direct_y],\
							B[t%2    ][x - 1 + STRIDE    ][_direct_y],\
							B[(t+1)%2][x - 1 + STRIDE * 2][_direct_y],\
							B[t%2    ][x - 1 + STRIDE * 3][_direct_y]);\
		vloadset(v_right_tmp,	B[(t+1)%2][x + 1             ][_direct_y],\
							B[t%2    ][x + 1 + STRIDE    ][_direct_y],\
							B[(t+1)%2][x + 1 + STRIDE * 2][_direct_y],\
							B[t%2    ][x + 1 + STRIDE * 3][_direct_y]);\
		row##_h1 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
		vloadset(v_left_tmp,	B[(t+1)%2][x - 2             ][_direct_y],\
							B[t%2    ][x - 2 + STRIDE    ][_direct_y],\
							B[(t+1)%2][x - 2 + STRIDE * 2][_direct_y],\
							B[t%2    ][x - 2 + STRIDE * 3][_direct_y]);\
		vloadset(v_right_tmp,	B[(t+1)%2][x + 2             ][_direct_y],\
							B[t%2    ][x + 2 + STRIDE    ][_direct_y],\
							B[(t+1)%2][x + 2 + STRIDE * 2][_direct_y],\
							B[t%2    ][x + 2 + STRIDE * 3][_direct_y]);\
		row##_h2 = _mm256_add_pd(v_left_tmp, v_right_tmp);\
	}

/* 根据 y 是否已经打包到 BV，在 BV loader 和 direct loader 之间选择。 */
#define load_feature_row_boundary(row, yy) {\
		int _boundary_y = (yy);\
		if (_boundary_y >= YSTART && _boundary_y <= last_packed_y) {\
			load_feature_row_from_bv(row, _boundary_y);\
		} else {\
			load_feature_row_direct(row, _boundary_y);\
		}\
	}

/* 把一个 x 列从普通 B 布局转置成时间向量，并写入指定 BV slice。 */
#define pack_b_column_to_bv(BVX, x_pos) {\
		for ( y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {\
			vload(v_center_0, B[t%2    ][(x_pos) + STRIDE *3][y]);\
			vload(v_center_1, B[(t+1)%2][(x_pos) + STRIDE *2][y]);\
			vload(v_center_2, B[t%2    ][(x_pos) + STRIDE   ][y]);\
			vload(v_center_3, B[(t+1)%2][(x_pos)            ][y]);\
			transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);\
			store_bv_aligned(BVX[(y - YSTART) + 3][0], v_center_3);\
			store_bv_aligned(BVX[(y - YSTART) + 2][0], v_center_2);\
			store_bv_aligned(BVX[(y - YSTART) + 1][0], v_center_1);\
			store_bv_aligned(BVX[(y - YSTART)    ][0], v_center_0);\
		}\
	}

/* 把一个 BV slice 中的时间向量反转置回普通 B 布局。 */
#define unpack_bv_column_to_b(BVX, x_pos) {\
		for ( y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN) {\
			load_bv_aligned(v_center_3, BVX[(y - YSTART) + 3][0]);\
			load_bv_aligned(v_center_2, BVX[(y - YSTART) + 2][0]);\
			load_bv_aligned(v_center_1, BVX[(y - YSTART) + 1][0]);\
			load_bv_aligned(v_center_0, BVX[(y - YSTART)    ][0]);\
			transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);\
			vstore(B[t%2    ][(x_pos) + STRIDE *3][y], v_center_0);\
			vstore(B[(t+1)%2][(x_pos) + STRIDE *2][y], v_center_1);\
			vstore(B[t%2    ][(x_pos) + STRIDE   ][y], v_center_2);\
			vstore(B[(t+1)%2][(x_pos)            ][y], v_center_3);\
		}\
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

/* 计算连续 4 个 y 输出；row_slot0..4 环形复用，避免整行窗口平移拷贝。 */
#define compute_25p_y_chunk(load_feature_row, yy) {\
		int _chunk_y = (yy);\
		load_feature_row(row_slot0, _chunk_y - 2);\
		load_feature_row(row_slot1, _chunk_y - 1);\
		load_feature_row(row_slot2, _chunk_y    );\
		load_feature_row(row_slot3, _chunk_y + 1);\
		load_feature_row(row_slot4, _chunk_y + 2);\
		compute_25p_feature_window(row_slot0, row_slot1, row_slot2, row_slot3, row_slot4);\
		Input_Output_1(out, v_result, in);\
		store_bv_aligned(BV5[_chunk_y - YSTART][0], v_result);\
		load_feature_row(row_slot0, _chunk_y + 3);\
		compute_25p_feature_window(row_slot1, row_slot2, row_slot3, row_slot4, row_slot0);\
		Input_Output_2(out, v_result, in);\
		store_bv_aligned(BV5[_chunk_y - YSTART + 1][0], v_result);\
		load_feature_row(row_slot1, _chunk_y + 4);\
		compute_25p_feature_window(row_slot2, row_slot3, row_slot4, row_slot0, row_slot1);\
		Input_Output_3(out, v_result, in);\
		store_bv_aligned(BV5[_chunk_y - YSTART + 2][0], v_result);\
		load_feature_row(row_slot2, _chunk_y + 5);\
		compute_25p_feature_window(row_slot3, row_slot4, row_slot0, row_slot1, row_slot2);\
		Input_Output_4(out, v_result, in);\
		store_bv_aligned(BV5[_chunk_y - YSTART + 3][0], v_result);\
	}

void vectime(double* A, int NX, int NY, int T) {
	
	double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;
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
	double (* BV_buffer) [bv_y_rows][VECLEN] = (double(*)[bv_y_rows][VECLEN])alloc_extra_array(sizeof(double) * bv_y_rows * VECLEN * 6);

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
		pack_b_column_to_bv(BV0, XSTART - 2);
		pack_b_column_to_bv(BV1, XSTART - 1);
		pack_b_column_to_bv(BV2, XSTART    );
		pack_b_column_to_bv(BV3, XSTART + 1);
		pack_b_column_to_bv(BV4, XSTART + 2);

		for ( x = XSTART; x <= last_vector_x; x++) {
			y = YSTART;

			/* y 低边界 chunk：显式 boundary loader，避免主路径分支。 */
			if (y <= last_full_chunk_y) {
				vload(in, B[t%2][x + STRIDE * VECLEN][y]);
				compute_25p_y_chunk(load_feature_row_boundary, y);
				vstore(B[t%2][x][y], out);
				y += VECLEN;
			}

			/* y 主路径：所有 stencil 行都在 BV 中，只走无分支 BV loader，并复用 5 行寄存器窗口。 */
			for ( ; y + VECLEN - 1 + YSLOPE <= last_packed_y; y += VECLEN) {
				/* in 是下一轮 x 迭代需要拼入 Input_Output 的右侧时间向量。 */
				vload(in, B[t%2][x + STRIDE * VECLEN][y]);
				compute_25p_y_chunk(load_feature_row_from_bv, y);
				vstore(B[t%2][x][y], out);
			}

			/* y 高边界 chunk：只处理剩余完整向量块。 */
			for ( ; y <= last_full_chunk_y; y += VECLEN) {
				vload(in, B[t%2][x + STRIDE * VECLEN][y]);
				compute_25p_y_chunk(load_feature_row_boundary, y);
				vstore(B[t%2][x][y], out);
			}

			/* y 尾部不进入 BV 输出 slice，继续 direct gather/scatter。 */
			for ( y = last_packed_y + 1; y < NY + YSTART; y++) {
				load_feature_row_boundary(row_slot0, y - 2);
				load_feature_row_boundary(row_slot1, y - 1);
				load_feature_row_boundary(row_slot2, y    );
				load_feature_row_boundary(row_slot3, y + 1);
				load_feature_row_boundary(row_slot4, y + 2);

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

		/* x 主体结束后，把最后 5 个输入 slice 从 extra array 转回普通 B 布局。 */
		unpack_bv_column_to_b(BV0, xx - 2);
		unpack_bv_column_to_b(BV1, xx - 1);
		unpack_bv_column_to_b(BV2, xx    );
		unpack_bv_column_to_b(BV3, xx + 1);
		unpack_bv_column_to_b(BV4, xx + 2);

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

#undef compute_25p_y_chunk
#undef compute_25p_feature_window
#undef unpack_bv_column_to_b
#undef pack_b_column_to_bv
#undef load_feature_row_boundary
#undef load_feature_row_direct
#undef load_feature_row_from_bv
#undef store_bv_aligned
#undef load_bv_aligned
