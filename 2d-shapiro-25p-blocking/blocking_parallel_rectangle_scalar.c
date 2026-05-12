#include "define.h"

void blocking_parallel_rectangle_scalar(double * A, int NX, int NY, int T,
										 int xb, int yb, int tb) {

	double (* B)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;

	int Bx = xb;
	int bx = Bx - 2 * tb * XSLOPE;
	int ix = Bx + bx;
	int nb0[2] = {myfloor(NX - Bx, ix), myfloor(NX - Bx, ix) + 1};
	int nrestpoints = NX % ix;
	int bx_first_B1 = (Bx + nrestpoints) / 2;
	int bx_last_B1 = (Bx + nrestpoints) - bx_first_B1;
	int xright[2] = {
		bx_first_B1 + Bx + XSTART,
		bx_first_B1 + (Bx - bx) / 2 + XSTART
	};

	int wave;
	int mylevel;
	int tt, xx, yy, t, x, y;
	int xmin, xmax, ybeg;

	int xblocknum = max(nb0[0], nb0[1]);
	int yblocknum = 2 * myceil(NY + YSLOPE * (T - 1), yb);
	int wavenum = myceil(T, tb) + 1 + myceil(NY + YSLOPE * (T - 1), yb);

	for (wave = 0; wave < wavenum; wave++) {
		#pragma omp parallel for private(mylevel, tt, t, x, y, xmin, xmax, ybeg) collapse(2) schedule(dynamic, 1)
		for (xx = 0; xx < xblocknum; xx++) {
			for (yy = 0; yy < yblocknum; yy++) {
				mylevel = (wave % 2 + yy) % 2;

				if (xx < nb0[mylevel]) {
					tt = -tb + (wave - yy) * tb;

					for (t = max(tt, 0); t < min(tt + 2 * tb, T); t++) {
						xmin = (mylevel == 1 && xx == 0) ?
							XSTART :
							(xright[mylevel] - Bx + xx * ix +
							 myabs((tt + tb), (t + 1)) * XSLOPE);
						xmax = (mylevel == 1 && xx == nb0[1] - 1) ?
							NX + XSTART :
							(xright[mylevel] + xx * ix -
							 myabs((tt + tb), (t + 1)) * XSLOPE);

						// 2d9p 的 y wavefront 推进按 1 格/步，这里改成 YSLOPE 格/步。
						ybeg = YSTART - wave * tb * YSLOPE +
							yy * (yb + tb * YSLOPE) -
							(t - tt) * YSLOPE;

						for (x = xmin; x < xmax; x++) {
							#pragma novector
							for (y = max(YSTART, ybeg);
								 y < min(NY + YSTART, ybeg + yb); y++) {
								Compute_scalar(B, t, x, y);
							}
						}
					}
				}
			}
		}
	}
}
