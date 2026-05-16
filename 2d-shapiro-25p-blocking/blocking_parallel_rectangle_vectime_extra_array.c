#include "define.h"

// 局部块上的 x-time extra-array 内核，外层分块负责 halo 拷贝和 owner 回写。
static void vectime_extra_array_kernel(double *A, int NX, int NY, int T,
									   double *AV_base)
{
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;
	double tmp[VECLEN];

	int tt, t = 0, x, xx, y;
	const int y_vec_last = YSTART + ((NY - VECLEN) / VECLEN) * VECLEN;
	const int y_vec_limit = y_vec_last + VECLEN - 1;
	const int y_tail_begin = y_vec_last + VECLEN;

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

	double (* AV)[NY][VECLEN] = (double (*)[NY][VECLEN])AV_base;

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

				for (y = YSTART; y <= y_vec_last; y += VECLEN) {
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
			if (y <= y_vec_last) {
				_mm_prefetch((const char *)
					&B[t%2][x + STRIDE * VECLEN + 1][y], _MM_HINT_T0);
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

				// 低边界 chunk 结束后，保留 y-2..y+1 四行给主路径复用。
				r3_c = r2_c;
				r3_d1 = r2_d1;
				r3_d2 = r2_d2;
				r2_c = r1_c;
				r2_d1 = r1_d1;
				r2_d2 = r1_d2;
				r1_c = r0_c;
				r1_d1 = r0_d1;
				r1_d2 = r0_d2;
				r0_c = r4_c;
				r0_d1 = r4_d1;
				r0_d2 = r4_d2;
			}

			// y 主路径：跨 chunk 复用上一轮留下的四行，只补 y+2..y+5。
			// 最后一个打包 y：BV 只保存完整 VECLEN 分块覆盖到的行。
			for ( ; y + VECLEN - 1 + YSLOPE <=
					y_vec_limit;
				  y += VECLEN) {
				// in 是下一轮 x 迭代需要拼入 Input_Output 的右侧时间向量。
				_mm_prefetch((const char *)
					&B[t%2][x + STRIDE * VECLEN + 1][y], _MM_HINT_T0);
				in = _mm256_loadu_pd(&B[t%2][x + STRIDE * VECLEN][y]);
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

				// 为下一个 y chunk 形成新的 y-2..y+1 窗口。
				r3_c = r2_c;
				r3_d1 = r2_d1;
				r3_d2 = r2_d2;
				r2_c = r1_c;
				r2_d1 = r1_d1;
				r2_d2 = r1_d2;
				r1_c = r0_c;
				r1_d1 = r0_d1;
				r1_d2 = r0_d2;
				r0_c = r4_c;
				r0_d1 = r4_d1;
				r0_d2 = r4_d2;
			}

			// y 高边界 chunk：只处理剩余完整向量块。
			for ( ; y <= y_vec_last;
				  y += VECLEN) {
				_mm_prefetch((const char *)
					&B[t%2][x + STRIDE * VECLEN + 1][y], _MM_HINT_T0);
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
			for ( y = y_tail_begin; y < NY + YSTART; y++) {
				if (y - 2 <= y_vec_limit) {
					load_sum_from_bv(r0_c, r0_d1, r0_d2, y - 2);
				} else {
					load_sum_direct(r0_c, r0_d1, r0_d2, y - 2);
				}

				if (y - 1 <= y_vec_limit) {
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

				for (y = YSTART; y <= y_vec_last; y += VECLEN) {
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

}

static void scalar_kernel(double *A, int NX, int NY, int T)
{
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;

	for (int t = 0; t < T; t++) {
		for (int x = XSTART; x < NX + XSTART; x++) {
			#pragma ivdep
			#pragma vector always
			for (int y = YSTART; y < NY + YSTART; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}
}


void blocking_parallel_rectangle_vectime_extra_array(double* A, int NX, int NY,
													 int T, int xb, int yb, int tb)
{
	int block_t = tb - tb % VECLEN;

	if (block_t < VECLEN) {
		block_t = VECLEN;
	}
	if (xb <= 0) {
		xb = NX;
	}
	if (yb <= 0) {
		yb = NY;
	}
	if (NX < STRIDE * VECLEN || NY < VECLEN ||
		xb < STRIDE * VECLEN || yb < VECLEN) {
		blocking_parallel_rectangle_scalar(A, NX, NY, T, xb, yb, tb);
		return;
	}

	/*
	 * The local-buffer wrapper recomputes halo.  Large user tb values make
	 * that halo dominate the owner tile, so keep the internal temporal tile
	 * small enough for the vector kernel to pay for itself.
	 */
	int shape_limited_block_t = min(32,
		min(max(VECLEN, xb / (4 * XSLOPE)),
			max(VECLEN, yb / (4 * YSLOPE))));
	shape_limited_block_t -= shape_limited_block_t % VECLEN;
	if (shape_limited_block_t < VECLEN) {
		shape_limited_block_t = VECLEN;
	}
	if (block_t > shape_limited_block_t) {
		block_t = shape_limited_block_t;
	}

	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;

	int max_x_halo = XSLOPE * block_t;
	int max_y_halo = YSLOPE * block_t;
	int max_local_NX = min(NX, xb + 2 * max_x_halo);
	int max_local_NY = min(NY, yb + 2 * max_y_halo);
	int xblocknum = myceil(NX, xb);
	int yblocknum = myceil(NY, yb);
	int threadnum = min(omp_get_max_threads(), xblocknum * yblocknum);

	threadnum = max(threadnum, 1);

	size_t max_lb_size = sizeof(double) * 2 *
		(max_local_NX + 2 * XSTART) * (max_local_NY + 2 * YSTART);
	size_t max_av_size = sizeof(double) * max_local_NY * VECLEN * 6;

	double (* SRC)[NY + 2 * YSTART] =
		(double (*)[NY + 2 * YSTART])
		malloc(sizeof(double) * (NX + 2 * XSTART) * (NY + 2 * YSTART));
	double **LB_pool = (double **)calloc(threadnum, sizeof(double *));
	double **AV_pool = (double **)calloc(threadnum, sizeof(double *));

	for (int tt = 0; tt < T; tt += block_t) {
		int dt = min(block_t, T - tt);
		int src = tt % 2;
		int dst = (tt + dt) % 2;
		int x_halo = XSLOPE * dt;
		int y_halo = YSLOPE * dt;

		memcpy(SRC, &B[src][0][0],
			   sizeof(double) * (NX + 2 * XSTART) * (NY + 2 * YSTART));

		#pragma omp parallel for collapse(2) schedule(static) num_threads(threadnum)
		for (int owner_begin = XSTART;
			 owner_begin < NX + XSTART;
			 owner_begin += xb) {
			for (int owner_y_begin = YSTART;
				 owner_y_begin < NY + YSTART;
				 owner_y_begin += yb) {
				int owner_end = min(owner_begin + xb, NX + XSTART);
				int owner_y_end = min(owner_y_begin + yb, NY + YSTART);
				int copy_begin = max(XSTART, owner_begin - x_halo);
				int copy_end = min(NX + XSTART, owner_end + x_halo);
				int copy_y_begin = max(YSTART, owner_y_begin - y_halo);
				int copy_y_end = min(NY + YSTART, owner_y_end + y_halo);
				int local_NX = copy_end - copy_begin;
				int local_NY = copy_y_end - copy_y_begin;
				int myid = omp_get_thread_num();

				if (LB_pool[myid] == NULL) {
					LB_pool[myid] = (double *)malloc(max_lb_size);
				}
				if (AV_pool[myid] == NULL) {
					AV_pool[myid] = (double *)alloc_extra_array(max_av_size);
				}

				double (* LB)[local_NX + 2 * XSTART][local_NY + 2 * YSTART] =
					(double (*)[local_NX + 2 * XSTART][local_NY + 2 * YSTART])
					LB_pool[myid];

				size_t local_row_size =
					sizeof(double) * (local_NY + 2 * YSTART);
				for (int lx = 0; lx < local_NX + 2 * XSTART; lx++) {
					int gx = copy_begin + lx - XSTART;
					int gy = copy_y_begin - YSTART;
					memcpy(&LB[0][lx][0], &SRC[gx][gy], local_row_size);
					memcpy(&LB[1][lx][0], &SRC[gx][gy], local_row_size);
				}

				if (local_NX < STRIDE * VECLEN || local_NY < VECLEN) {
					scalar_kernel((double *)LB, local_NX, local_NY, dt);
				} else {
					vectime_extra_array_kernel((double *)LB, local_NX,
						local_NY, dt, AV_pool[myid]);
				}

				int local_parity = dt % 2;
				for (int gx = owner_begin; gx < owner_end; gx++) {
					int lx = XSTART + gx - copy_begin;
					int ly = YSTART + owner_y_begin - copy_y_begin;
					memcpy(&B[dst][gx][owner_y_begin],
						   &LB[local_parity][lx][ly],
						   sizeof(double) * (owner_y_end - owner_y_begin));
				}
			}
		}
	}

	for (int i = 0; i < threadnum; i++) {
		free(LB_pool[i]);
		free_extra_array(AV_pool[i]);
	}
	free(LB_pool);
	free(AV_pool);
	free(SRC);
}
