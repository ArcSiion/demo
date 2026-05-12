#include "defines.h"

void blocking_parallel_rectangle_vector(double * A, int NX, int NY, int NZ, int T, int xb, int yb, int zb, int tb){


	double (* B)[NX + 2 * XSTART][ NY+2*YSTART][ NZ+2*ZSTART] =  (double (*)[NX + 2 * XSTART][ NY+2*YSTART][ NZ+2*ZSTART])  A;

	int Bx = xb;
	int bx = Bx - 2 * tb * XSLOPE;
	int ix = Bx + bx;   // ix is even
	int nb0[2] = { myfloor(NX-Bx,ix), myfloor(NX-Bx,ix) + 1 };	
	int nrestpoints = NX % ix;
	int bx_first_B1 = (Bx + nrestpoints)/2;
	int bx_last_B1  = (Bx + nrestpoints) - bx_first_B1;
	int xright[2] = {bx_first_B1 + Bx + XSTART,  bx_first_B1 + (Bx - bx)/2 + XSTART};

	int wave;
	int mylevel;
	int tt, xx, yy, zz, t, x, y, z;
	int xmin, xmax, ybeg, zbeg;

	const int xblocknum = max(nb0[0], nb0[1]);
	const int yblocknum = 2 * myceil(NY + T - 1, yb);
	const int zblocknum = 2 * myceil(NZ + T - 1, zb);

	
	for (wave = 0; wave < myceil(T, tb) + 1 + myceil(NY + T - 1, yb) + myceil(NZ + T - 1, zb); wave++) {

		#pragma omp parallel for private(mylevel, t, x, y, z, xx, yy, zz, tt, xmin, xmax, ybeg, zbeg) collapse(3) schedule(dynamic, 1)


		for (xx = 0; xx < xblocknum; xx++){
			for (yy = 0; yy < yblocknum; yy++){
				for (zz = 0; zz < zblocknum; zz++){

					mylevel = (wave % 2 + yy + zz) % 2;

					if(xx < nb0[mylevel]){

						tt = -tb + (wave - yy - zz) * tb;

						for (t = max(tt, 0); t < min(tt + 2 * tb, T); t++){
							xmin = (mylevel == 1 && xx == 0) ?	             XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
							xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
							ybeg = YSTART + yy * yb - (wave - yy - zz) * tb - (t - tt);
							zbeg = ZSTART + zz * zb - (wave - yy - zz) * tb - (t - tt);

							for (x = xmin; x < xmax; x++) {

								for (y = max(YSTART, ybeg); y < min(NY + YSTART, ybeg + yb); y++) {
									#pragma vector always
									#pragma ivdep
									for (z = max(ZSTART, zbeg); z < min(NZ + ZSTART, zbeg + zb); z++) {
									
										Compute_scalar(B, t, x, y, z);
									}
								}
							}
						}	
					}
				}				
			}
		}
	}
}