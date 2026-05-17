#include "define.h"

static inline __attribute__((always_inline)) void true_wavefront_back_scalar(
	double *A, int NX, int NY, int t_begin, int t_end, int tt, int wave,
	int xx, int yy, int mylevel, int Bx, int ix, int nb1, int xright0,
	int xright1, int yb, int block_t)
{
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;
	int xright[2] = {xright0, xright1};
	int t, x, y;
	int xmin, xmax, ybeg, ylo, yhi;

	for (t = t_begin; t < t_end; t++) {
		xmin = (mylevel == 1 && xx == 0) ?
			XSTART :
			(xright[mylevel] - Bx + xx * ix +
			 myabs((tt + block_t), (t + 1)) * XSLOPE);
		xmax = (mylevel == 1 && xx == nb1 - 1) ?
			NX + XSTART :
			(xright[mylevel] + xx * ix -
			 myabs((tt + block_t), (t + 1)) * XSLOPE);

		ybeg = YSTART - wave * block_t * YSLOPE +
			yy * (yb + block_t * YSLOPE) -
			(t - tt) * YSLOPE;
		ylo = max(YSTART, ybeg);
		yhi = min(NY + YSTART, ybeg + yb);

		for (x = xmin; x < xmax; x++) {
			#pragma vector always
			#pragma ivdep
			for (y = ylo; y < yhi; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}
}

void blocking_parallel_rectangle_vectime_true_wavefront(double *A, int NX, int NY,
														int T, int xb, int yb, int tb)
{
	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;
	int block_t = tb;

	if (block_t <= 0) {
		blocking_parallel_rectangle_vector(A, NX, NY, T, xb, yb, tb);
		return;
	}
	if (xb <= 0) {
		xb = NX;
	}
	if (yb <= 0) {
		yb = NY;
	}
	if (xb <= 2 * block_t * XSLOPE) {
		blocking_parallel_rectangle_vector(A, NX, NY, T, xb, yb, tb);
		return;
	}

	int Bx = xb;
	int bx = Bx - 2 * block_t * XSLOPE;
	int ix = Bx + bx;
	int nb0[2] = {myfloor(NX - Bx, ix), myfloor(NX - Bx, ix) + 1};
	int nrestpoints = NX % ix;
	int bx_first_B1 = (Bx + nrestpoints) / 2;
	int xright[2] = {
		bx_first_B1 + Bx + XSTART,
		bx_first_B1 + (Bx - bx) / 2 + XSTART
	};

	int mylevel;
	int tt, xx, yy, tv, t, x, y;
	int xmin, xmax, ybeg, ylo, yhi;
	int ybeg_first, ybeg_last;

	int xblocknum = max(nb0[0], nb0[1]);
	int yblocknum = 2 * myceil(NY + YSLOPE * (T - 1), yb);
	int tblocknum = myceil(T, block_t);
	int wavenum = myceil(T, block_t) + 1 +
		myceil(NY + YSLOPE * (T - 1), yb);

	#pragma omp parallel private(mylevel, tt, tv, t, x, y, xmin, xmax, ybeg, ylo, yhi, ybeg_first, ybeg_last)
	{
		for (int wave = 0; wave < wavenum; wave++) {
			int yy_begin = max(0, wave - tblocknum);
			int yy_end = min(yblocknum, wave + 1);

			#pragma omp for collapse(2) schedule(dynamic, 1)
			for (xx = 0; xx < xblocknum; xx++) {
				for (yy = yy_begin; yy < yy_end; yy++) {
					mylevel = (wave % 2 + yy) % 2;

					if (xx < nb0[mylevel]) {
						int t_begin, t_end;

						tt = -block_t + (wave - yy) * block_t;
						t_begin = max(tt, 0);
						t_end = min(tt + 2 * block_t, T);
						ybeg_first = YSTART - wave * block_t * YSLOPE +
							yy * (yb + block_t * YSLOPE) -
							(t_begin - tt) * YSLOPE;
						ybeg_last = YSTART - wave * block_t * YSLOPE +
							yy * (yb + block_t * YSLOPE) -
							(t_end - 1 - tt) * YSLOPE;

						if (t_begin >= t_end ||
							ybeg_first + yb <= YSTART ||
							ybeg_last >= NY + YSTART) {
							continue;
						}

						#ifdef SHAPIRO_TRUE_WAVEFRONT_TILE
						for (tv = t_begin; tv <= t_end - VECLEN; tv += VECLEN) {
							true_wavefront_back_scalar(
								A, NX, NY, tv, tv + VECLEN, tt, wave,
								xx, yy, mylevel, Bx, ix, nb0[1],
								xright[0], xright[1], yb, block_t);
						}
						if (tv < t_end) {
							true_wavefront_back_scalar(
								A, NX, NY, tv, t_end, tt, wave,
								xx, yy, mylevel, Bx, ix, nb0[1],
								xright[0], xright[1], yb, block_t);
						}
						#else
						for (t = t_begin; t < t_end; t++) {
							xmin = (mylevel == 1 && xx == 0) ?
								XSTART :
								(xright[mylevel] - Bx + xx * ix +
								 myabs((tt + block_t), (t + 1)) * XSLOPE);
							xmax = (mylevel == 1 && xx == nb0[1] - 1) ?
								NX + XSTART :
								(xright[mylevel] + xx * ix -
								 myabs((tt + block_t), (t + 1)) * XSLOPE);

							ybeg = YSTART - wave * block_t * YSLOPE +
								yy * (yb + block_t * YSLOPE) -
								(t - tt) * YSLOPE;
							ylo = max(YSTART, ybeg);
							yhi = min(NY + YSTART, ybeg + yb);

							for (x = xmin; x < xmax; x++) {
								#pragma vector always
								#pragma ivdep
								for (y = ylo; y < yhi; y++) {
									Compute_scalar(B, t, x, y);
								}
							}
						}
						#endif
					}
				}
			}
		}
	}
}
