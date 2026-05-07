#include "define.h"

#define load_time_vector(v, dx, dy) vloadset(v, B[(t+1)%2][x+(dx)][y+(dy)],\
											 B[(t)%2  ][x+STRIDE+(dx)][y+(dy)],\
											 B[(t+1)%2][x+STRIDE*2+(dx)][y+(dy)],\
											 B[(t)%2  ][x+STRIDE*3+(dx)][y+(dy)])

void vectime(double* A, int NX, int NY, int T) {
	double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;
	double tmp[4];

	int tt, t = 0, x, xx, y;
	vec v_m2_m2, v_m1_m2, v_0_m2, v_p1_m2, v_p2_m2;
	vec v_m2_m1, v_m1_m1, v_0_m1, v_p1_m1, v_p2_m1;
	vec v_m2_0,  v_m1_0,  v_0_0,  v_p1_0,  v_p2_0;
	vec v_m2_p1, v_m1_p1, v_0_p1, v_p1_p1, v_p2_p1;
	vec v_m2_p2, v_m1_p2, v_0_p2, v_p1_p2, v_p2_p2;
	vec vout;
	SET_COFF;

	for ( tt = 0; tt <= T - VECLEN; tt += VECLEN){

		for( t = tt ; t < tt + VECLEN - 1 ; t++){
			for ( x = XSTART; x < XSTART + STRIDE * (VECLEN - 1 - (t - tt)); x++) {
            #pragma ivdep
            #pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}

		t = tt;
		for ( x = XSTART ; x <= NX + XSTART - STRIDE * VECLEN ; x ++){
			for ( y = YSTART; y < NY + YSTART; y++) {
				load_time_vector(v_m2_m2, -2, -2);
				load_time_vector(v_m1_m2, -1, -2);
				load_time_vector(v_0_m2,   0, -2);
				load_time_vector(v_p1_m2,  1, -2);
				load_time_vector(v_p2_m2,  2, -2);

				load_time_vector(v_m2_m1, -2, -1);
				load_time_vector(v_m1_m1, -1, -1);
				load_time_vector(v_0_m1,   0, -1);
				load_time_vector(v_p1_m1,  1, -1);
				load_time_vector(v_p2_m1,  2, -1);

				load_time_vector(v_m2_0,  -2,  0);
				load_time_vector(v_m1_0,  -1,  0);
				load_time_vector(v_0_0,    0,  0);
				load_time_vector(v_p1_0,   1,  0);
				load_time_vector(v_p2_0,   2,  0);

				load_time_vector(v_m2_p1, -2,  1);
				load_time_vector(v_m1_p1, -1,  1);
				load_time_vector(v_0_p1,   0,  1);
				load_time_vector(v_p1_p1,  1,  1);
				load_time_vector(v_p2_p1,  2,  1);

				load_time_vector(v_m2_p2, -2,  2);
				load_time_vector(v_m1_p2, -1,  2);
				load_time_vector(v_0_p2,   0,  2);
				load_time_vector(v_p1_p2,  1,  2);
				load_time_vector(v_p2_p2,  2,  2);

				Compute_1vector(v_m2_m2, v_m1_m2, v_0_m2, v_p1_m2, v_p2_m2,\
								v_m2_m1, v_m1_m1, v_0_m1, v_p1_m1, v_p2_m1,\
								v_m2_0,  v_m1_0,  v_0_0,  v_p1_0,  v_p2_0,\
								v_m2_p1, v_m1_p1, v_0_p1, v_p1_p1, v_p2_p1,\
								v_m2_p2, v_m1_p2, v_0_p2, v_p1_p2, v_p2_p2,\
								vout);

				_mm256_storeu_pd(tmp, vout);
				B[(t+1)%2][x + STRIDE * 3][ y ] = tmp[0];
				B[t%2    ][x + STRIDE * 2][ y ] = tmp[1];
				B[(t+1)%2][x + STRIDE * 1][ y ] = tmp[2];
				B[t%2    ][x             ][ y ] = tmp[3];
			}
		}

		xx = x;
		for( t = tt ; t < tt + VECLEN ; t++){
			for ( x = xx + STRIDE * (VECLEN - 1 - (t - tt)); x < NX + XSTART; x++) {
            #pragma ivdep
            #pragma vector always
				for ( y = YSTART; y < NY + YSTART; y++) {
					Compute_scalar(B, t, x, y);
				}
			}
		}
	}

	for ( ; t < T; t++){
		for (x = XSTART; x < NX + XSTART; x++) {
        #pragma ivdep
        #pragma vector always
			for ( y = YSTART; y < NY + YSTART; y++) {
				Compute_scalar(B, t, x, y);
			}
		}
	}
}

#undef load_time_vector
