#include "define_512.h"

void blocking_parallel_rectangle_vectime_512_hybrid(double *A, int NX, int NY, int T, int xb, int yb, int tb)
{

	double(*B)[NX + 2 * XSTART][NY + 2 * YSTART] = (double(*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;

	int tt, t, tv, x, xx, y, yy;

	tb -= tb % VVECLEN;
	if (tb < VVECLEN) tb = VVECLEN;
	int Bx = xb;
	int bx = Bx - 2 * tb * XSLOPE;
	int ix = Bx + bx;
	int nb0[2] = {myfloor(NX - Bx, ix), myfloor(NX - Bx, ix) + 1};
	int nrestpoints = NX % ix;
	int bx_first_B1 = (Bx + nrestpoints) / 2;
	int bx_last_B1 = (Bx + nrestpoints) - bx_first_B1;
	int xright[2] = {bx_first_B1 + Bx + XSTART, bx_first_B1 + (Bx - bx) / 2 + XSTART};

	int wave;
	int mylevel;
	int xmin, xmax, ybeg;
    int myybeg, myyb;

	for (wave = 0; wave < myceil(T, tb) + 1 + myceil(NY + T - 1, yb); wave++) { 
		#pragma omp parallel for private(mylevel, tt, tv, t, x, y, xmin, xmax, ybeg, myybeg, myyb)  collapse(2) schedule(dynamic, 1)

		for (xx = 0; xx < max(nb0[0], nb0[1]); xx++) {

			for (yy = 0; yy < 2 * myceil(NY + T - 1, yb); yy++) {

				__m512d v_x_plus_0, v_x_plus_1, v_x_plus_2, v_x_plus_3, v_x_plus_4, v_x_plus_5, v_x_plus_6, v_x_plus_7;
				__m512d v_x_minus_0, v_x_minus_1, v_x_minus_2, v_x_minus_3, v_x_minus_4, v_x_minus_5, v_x_minus_6, v_x_minus_7;
				__m512d v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7;
				__m512d in, out;
				SET_COFF_512;
				double tmp[8];

				mylevel = (wave % 2 + yy) % 2;
                
				if (xx < nb0[mylevel]) {

					tt = -tb + (wave - yy) * tb;

                    ybeg = YSTART - wave * tb + yy * (yb + tb);

#ifdef DEBUG_512
if (omp_get_thread_num() == 0) {
    printf("W=%d xx=%d yy=%d ml=%d tt=%d ybeg=%d nb0=[%d,%d] Bx=%d bx=%d ix=%d xright=[%d,%d]\n",
           wave, xx, yy, mylevel, tt, ybeg, nb0[0], nb0[1], Bx, bx, ix, xright[0], xright[1]);
}
#endif
					
					for (tv = max(tt, 0); tv <= min(tt + 2 * tb, T) - VVECLEN; tv += VVECLEN, ybeg -= VVECLEN) {
					#ifdef DEBUG_512
					if (omp_get_thread_num() == 0) {
					    printf("  tv=%d ybeg=%d", tv, ybeg);
					}
					#endif
						
                        if(ybeg - VVECLEN + 1 < YSTART){
					        myybeg 	= 	YSTART + VVECLEN - 1;
					        myyb	=	yb - (myybeg - ybeg);
					    } else {
					    	myybeg	=	ybeg;
					    	myyb	=	yb;
					    }
					    if (ybeg + yb >= YSTART + NY){	
					    	myyb -= ybeg + yb - (YSTART + NY);
					    }
                        if (myyb <= 2 * VVECLEN ) {
    #ifdef DEBUG_512
    if (omp_get_thread_num() == 0) { printf(" myybeg=%d myyb=%d SCALAR\n", myybeg, myyb); }
    #endif
						    for (t = tv; t < tv + VVECLEN; t++){
						    	xmin = (mylevel == 1 && xx == 0) ?	             XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
						        xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
								
						    	for (x = xmin; x < xmax; x++) {
						    		#pragma ivdep
									#pragma vector always
									for (y = max(YSTART, ybeg - (t - tv)); y < min(NY + YSTART, ybeg - (t - tv) + yb); y++) {
						    			Compute_scalar(B, t, x, y);
										
						    		}
						    	}
						    }
                        }
                        else {
#ifdef DEBUG_512
if (omp_get_thread_num() == 0) { printf(" myybeg=%d myyb=%d VECTOR\n", myybeg, myyb); }
#endif
						    //head
							for (t = tv; t < tv + VVECLEN - 1; t++){

						    	xmin = (mylevel == 1 && xx == 0) ?	             XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
						        xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
								
						    	for (x = xmin; x < xmax; x++) {

						    		#pragma ivdep
									#pragma vector always
									for (y = max(YSTART, ybeg - (t - tv)); y < myybeg - (t - tv); y++) {
						    			Compute_scalar(B, t, x, y);

						    		}
						    	}
						    }

						    for (t = tv; t < tv + VVECLEN; t++) {

						    	xmin = (mylevel == 1 && xx == 0) ? XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
						    	xmax = xmin + STRIDE * (VVECLEN - 1 - (t - tv));
						    	if (!(mylevel == 1 && xx == 0)) {
						    		xmax += ((tt + tb < tv + 1) ? 1 : -1) * (VVECLEN - 1 - (t - tv));
						    	}
						    	for (x = xmin; x < xmax + 2; x++) {

						    		#pragma vector always
						    		#pragma ivdep
						    		// AVX-256 9-point hybrid: N,E,W,S,NW,NE,SW,SE order
					__m256d vc0 = _mm256_set1_pd(C0);
					__m256d vc1 = _mm256_set1_pd(C1);
					for (y = myybeg - (t - tv); y + 3 < myybeg - (t - tv) + myyb; y += 4){
						__m256d vn  = _mm256_loadu_pd(&B[t%2][x][y-1]);
						__m256d ve  = _mm256_loadu_pd(&B[t%2][x+1][y]);
						__m256d vw  = _mm256_loadu_pd(&B[t%2][x-1][y]);
						__m256d vs  = _mm256_loadu_pd(&B[t%2][x][y+1]);
						__m256d vnw = _mm256_loadu_pd(&B[t%2][x-1][y-1]);
						__m256d vne = _mm256_loadu_pd(&B[t%2][x+1][y-1]);
						__m256d vsw = _mm256_loadu_pd(&B[t%2][x-1][y+1]);
						__m256d vse = _mm256_loadu_pd(&B[t%2][x+1][y+1]);
						__m256d vm  = _mm256_loadu_pd(&B[t%2][x][y]);
						__m256d stub = _mm256_add_pd(vn, ve);
						stub = _mm256_add_pd(stub, vw);
						stub = _mm256_add_pd(stub, vs);
						stub = _mm256_add_pd(stub, vnw);
						stub = _mm256_add_pd(stub, vne);
						stub = _mm256_add_pd(stub, vsw);
						stub = _mm256_add_pd(stub, vse);
						__m256d res = _mm256_fmadd_pd(vc0, vm, _mm256_mul_pd(vc1, stub));
						_mm256_storeu_pd(&B[(t+1)%2][x][y], res);
					}
					for (; y < myybeg - (t - tv) + myyb; y++){
						Compute_scalar(B, t, x, y);
					}
						    	}
						    }

						    t = tv;

						    xmin += 1;
						    xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel] + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
						    
							for (x = xmin; x < xmin + STRIDE + 1; x++) {

						    	for (y = myybeg - VVECLEN + 1; y <= myybeg - VVECLEN + 1 + myyb - VVECLEN; y += VVECLEN) {

						    		vload_512_blk(v_center_7, B[(t + 1) % 2][x][y]);
						    		vload_512_blk(v_center_6, B[(t) % 2][x + STRIDE][y+1]);
						    		vload_512_blk(v_center_5, B[(t + 1) % 2][x + STRIDE * 2][y+2]);
						    		vload_512_blk(v_center_4, B[(t) % 2][x + STRIDE * 3][y+3]);
						    		vload_512_blk(v_center_3, B[(t + 1) % 2][x + STRIDE * 4][y+4]);
						    		vload_512_blk(v_center_2, B[(t) % 2][x + STRIDE * 5][y+5]);
						    		vload_512_blk(v_center_1, B[(t + 1) % 2][x + STRIDE * 6][y+6]);
						    		vload_512_blk(v_center_0, B[(t) % 2][x + STRIDE * 7][y+7]);

						    		transpose_512(v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7, in, out);

						    		vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 0][y], v_center_0);
						    		vstore_512_blk(B[(t) % 2][x + STRIDE * 1][y+1], v_center_1);
						    		vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 2][y+2], v_center_2);
						    		vstore_512_blk(B[(t) % 2][x + STRIDE * 3][y+3], v_center_3);
						    		vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 4][y+4], v_center_4);
						    		vstore_512_blk(B[(t) % 2][x + STRIDE * 5][y+5], v_center_5);
						    		vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 6][y+6], v_center_6);
						    		vstore_512_blk(B[(t) % 2][x + STRIDE * 7][y+7], v_center_7);
						    	}
						    }
						    for (x = xmin + XSLOPE; x < xmax - STRIDE * VVECLEN - 1; x++) {

						    	y = myybeg - VVECLEN + 1;

						    	v_center_0 = _mm512_set_pd(B[(t + 1) % 2][x             ][y - 1],\
                                                           B[(t)     % 2][x + STRIDE    ][y    ],\
                                                           B[(t + 1) % 2][x + STRIDE * 2][y + 1], \
                                                           B[(t)     % 2][x + STRIDE * 3][y + 2], \
                                                           B[(t + 1) % 2][x + STRIDE * 4][y + 3], \
                                                           B[(t)     % 2][x + STRIDE * 5][y + 4], \
                                                           B[(t + 1) % 2][x + STRIDE * 6][y + 5], \
                                                           B[(t)     % 2][x + STRIDE * 7][y + 6]);
						    	v_x_plus_0 = _mm512_set_pd(B[(t + 1) % 2][x + 1            ][y - 1],\
                                                           B[(t)     % 2][x + 1 + STRIDE    ][y    ],\
                                                           B[(t + 1) % 2][x + 1 + STRIDE * 2][y + 1], \
                                                           B[(t)     % 2][x + 1 + STRIDE * 3][y + 2], \
                                                           B[(t + 1) % 2][x + 1 + STRIDE * 4][y + 3], \
                                                           B[(t)     % 2][x + 1 + STRIDE * 5][y + 4], \
                                                           B[(t + 1) % 2][x + 1 + STRIDE * 6][y + 5], \
                                                           B[(t)     % 2][x + 1 + STRIDE * 7][y + 6]);
						    	v_x_minus_0 = _mm512_set_pd(B[(t + 1) % 2][x - 1             ][y - 1],\
                                                           B[(t)     % 2][x - 1 + STRIDE    ][y    ],\
                                                           B[(t + 1) % 2][x - 1 + STRIDE * 2][y + 1], \
                                                           B[(t)     % 2][x - 1 + STRIDE * 3][y + 2], \
                                                           B[(t + 1) % 2][x - 1 + STRIDE * 4][y + 3], \
                                                           B[(t)     % 2][x - 1 + STRIDE * 5][y + 4], \
                                                           B[(t + 1) % 2][x - 1 + STRIDE * 6][y + 5], \
                                                           B[(t)     % 2][x - 1 + STRIDE * 7][y + 6]);

						    	v_center_7    = _mm512_set_pd(B[(t + 1) % 2][x + STRIDE    ][y    ],\
                                                           B[(t)     % 2][x + STRIDE * 2][y + 1],\
                                                           B[(t + 1) % 2][x + STRIDE * 3][y + 2], \
                                                           B[(t)     % 2][x + STRIDE * 4][y + 3], \
                                                           B[(t + 1) % 2][x + STRIDE * 5][y + 4], \
                                                           B[(t)     % 2][x + STRIDE * 6][y + 5], \
                                                           B[(t + 1) % 2][x + STRIDE * 7][y + 6], \
                                                           B[(t)     % 2][x + STRIDE * 8][y + 7]);

						    	vload_512_blk(v_center_1, B[(t + 1) % 2][x][y]);
						    	vload_512_blk(v_x_minus_1, B[(t + 1) % 2][x - XSLOPE][y]);
						    	vload_512_blk(v_x_plus_1, B[(t + 1) % 2][x + XSLOPE][y]);

						    	for (; y <= myybeg - VVECLEN + 1 + myyb - VVECLEN; y += VVECLEN) {

						    		vload_512_blk(in, B[(t) % 2][x + STRIDE * 8][y + VVECLEN]); // the next x iter in vector

									vstore_512_blk(B[(t + 1) % 2][x + STRIDE][y], v_center_7);

						    		vload_512_blk(v_x_minus_2, B[(t) % 2][x - XSLOPE + STRIDE][y+1]);
						    		vload_512_blk(v_x_plus_2, B[(t) % 2][x + XSLOPE + STRIDE][y+1]);
						    		vload_512_blk(v_center_2, B[(t) % 2][x + STRIDE][y+1]);
									Compute_1vector_512(v_x_minus_2, v_center_2, v_x_plus_2,\
															v_x_minus_1, v_center_1, v_x_plus_1,\
															v_x_minus_0, v_center_0, v_x_plus_0);
						    		Input_Output_1_512(out, v_center_0, in);
									vstore_512_blk(B[(t) % 2][x + STRIDE * 2][y+1], v_center_0);

						    		vload_512_blk(v_x_minus_3, B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y+2]);
						    		vload_512_blk(v_x_plus_3, B[(t + 1) % 2][x + XSLOPE + STRIDE * 2][y+2]);
						    		vload_512_blk(v_center_3, B[(t + 1) % 2][x + STRIDE * 2][y+2]);
									Compute_1vector_512(v_x_minus_3, v_center_3, v_x_plus_3,\
															v_x_minus_2, v_center_2, v_x_plus_2,\
															v_x_minus_1, v_center_1, v_x_plus_1);
						    		Input_Output_2_512(out, v_center_1, in);
									vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 3][y+2], v_center_1);

						    		vload_512_blk(v_x_minus_4, B[(t) % 2][x - XSLOPE + STRIDE * 3][y+3]);
						    		vload_512_blk(v_x_plus_4, B[(t) % 2][x + XSLOPE + STRIDE * 3][y+3]);
						    		vload_512_blk(v_center_4, B[(t) % 2][x + STRIDE * 3][y+3]);
									Compute_1vector_512(v_x_minus_4, v_center_4, v_x_plus_4,\
															v_x_minus_3, v_center_3, v_x_plus_3,\
															v_x_minus_2, v_center_2, v_x_plus_2);
						    		Input_Output_3_512(out, v_center_2, in);
									vstore_512_blk(B[(t) % 2][x + STRIDE * 4][y+3], v_center_2);

						    		vload_512_blk(v_x_minus_5, B[(t + 1) % 2][x - XSLOPE + STRIDE * 4][y+4]);
						    		vload_512_blk(v_x_plus_5, B[(t + 1) % 2][x + XSLOPE + STRIDE * 4][y+4]);
						    		vload_512_blk(v_center_5, B[(t + 1) % 2][x + STRIDE * 4][y+4]);
									Compute_1vector_512(v_x_minus_5, v_center_5, v_x_plus_5,\
															v_x_minus_4, v_center_4, v_x_plus_4,\
															v_x_minus_3, v_center_3, v_x_plus_3);
						    		Input_Output_4_512(out, v_center_3, in);
									vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 5][y+4], v_center_3);

						    		vload_512_blk(v_x_minus_6, B[(t) % 2][x - XSLOPE + STRIDE * 5][y+5]);
						    		vload_512_blk(v_x_plus_6, B[(t) % 2][x + XSLOPE + STRIDE * 5][y+5]);
						    		vload_512_blk(v_center_6, B[(t) % 2][x + STRIDE * 5][y+5]);
									Compute_1vector_512(v_x_minus_6, v_center_6, v_x_plus_6,\
															v_x_minus_5, v_center_5, v_x_plus_5,\
															v_x_minus_4, v_center_4, v_x_plus_4);
						    		Input_Output_5_512(out, v_center_4, in);
									vstore_512_blk(B[(t) % 2][x + STRIDE * 6][y+5], v_center_4);

						    		vload_512_blk(v_x_minus_7, B[(t + 1) % 2][x - XSLOPE + STRIDE * 6][y+6]);
						    		vload_512_blk(v_x_plus_7, B[(t + 1) % 2][x + XSLOPE + STRIDE * 6][y+6]);
						    		vload_512_blk(v_center_7, B[(t + 1) % 2][x + STRIDE * 6][y+6]);
									Compute_1vector_512(v_x_minus_7, v_center_7, v_x_plus_7,\
															v_x_minus_6, v_center_6, v_x_plus_6,\
															v_x_minus_5, v_center_5, v_x_plus_5);
						    		Input_Output_6_512(out, v_center_5, in);
									vstore_512_blk(B[(t + 1) % 2][x + STRIDE * 7][y+6], v_center_5);

						    		vload_512_blk(v_x_minus_0, B[(t) % 2][x - XSLOPE + STRIDE * 7][y+7]);
						    		vload_512_blk(v_x_plus_0, B[(t) % 2][x + XSLOPE + STRIDE * 7][y+7]);
						    		vload_512_blk(v_center_0, B[(t) % 2][x + STRIDE * 7][y+7]);
									Compute_1vector_512(v_x_minus_0, v_center_0, v_x_plus_0,\
															v_x_minus_7, v_center_7, v_x_plus_7,\
															v_x_minus_6, v_center_6, v_x_plus_6);
						    		Input_Output_7_512(out, v_center_6, in);
									vstore_512_blk(B[(t) % 2][x + STRIDE * 8][y+7], v_center_6);

						    		v_x_plus_1 = (y > myybeg - VVECLEN + 1 + myyb - VVECLEN - VVECLEN) ?  \
						    		                                      (_mm512_set_pd(	B[(t + 1) % 2][x + XSLOPE][y + VVECLEN], \
						    							B[(t) % 2][x + XSLOPE + STRIDE][y + VVECLEN + 1], \
						    							B[(t + 1) % 2][x + XSLOPE + STRIDE * 2][y + VVECLEN + 2], \
						    							B[(t) % 2][x + XSLOPE + STRIDE * 3][y + VVECLEN + 3], \
						    							B[(t + 1) % 2][x + XSLOPE + STRIDE * 4][y + VVECLEN + 4], \
						    							B[(t) % 2][x + XSLOPE + STRIDE * 5][y + VVECLEN + 5], \
						    							B[(t + 1) % 2][x + XSLOPE + STRIDE * 6][y + VVECLEN + 6], \
						    							B[(t) % 2][x + XSLOPE + STRIDE * 7][y + VVECLEN + 7])) \
						    				: _mm512_loadu_pd(&B[(t + 1) % 2][x + XSLOPE][y + VVECLEN]);
						    		v_x_minus_1 = (y > myybeg - VVECLEN + 1 + myyb - VVECLEN - VVECLEN) ?  \
						    		                                      (_mm512_set_pd(	B[(t + 1) % 2][x - XSLOPE][y + VVECLEN], \
						    							B[(t) % 2][x - XSLOPE + STRIDE][y + VVECLEN + 1], \
						    							B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y + VVECLEN + 2], \
						    							B[(t) % 2][x - XSLOPE + STRIDE * 3][y + VVECLEN + 3], \
						    							B[(t + 1) % 2][x - XSLOPE + STRIDE * 4][y + VVECLEN + 4], \
						    							B[(t) % 2][x - XSLOPE + STRIDE * 5][y + VVECLEN + 5], \
						    							B[(t + 1) % 2][x - XSLOPE + STRIDE * 6][y + VVECLEN + 6], \
						    							B[(t) % 2][x - XSLOPE + STRIDE * 7][y + VVECLEN + 7])) \
						    				: _mm512_loadu_pd(&B[(t + 1) % 2][x - XSLOPE][y + VVECLEN]);
						    		 v_center_1 = (y > myybeg - VVECLEN + 1 + myyb - VVECLEN - VVECLEN) ?  \
                                                    (_mm512_set_pd(	B[(t + 1) % 2][x][y + VVECLEN], \
																										B[(t) % 2][x + STRIDE][y + VVECLEN + 1], \
																										B[(t + 1) % 2][x + STRIDE * 2][y + VVECLEN + 2], \
																										B[(t) % 2][x + STRIDE * 3][y + VVECLEN + 3], \
																										B[(t + 1) % 2][x + STRIDE * 4][y + VVECLEN + 4], \
																										B[(t) % 2][x + STRIDE * 5][y + VVECLEN + 5], \
																										B[(t + 1) % 2][x + STRIDE * 6][y + VVECLEN + 6], \
																										B[(t) % 2][x + STRIDE * 7][y + VVECLEN + 7])) \
						    						: _mm512_loadu_pd(&B[(t + 1) % 2][x][y + VVECLEN]);
						    		Compute_1vector_512(v_x_minus_1, v_center_1, v_x_plus_1,\
															v_x_minus_0, v_center_0, v_x_plus_0,\
															v_x_minus_7, v_center_7, v_x_plus_7);
									
									if (y >  myybeg - VVECLEN + 1 + myyb - VVECLEN - VVECLEN) {
										_mm512_storeu_pd(tmp, v_center_7);
										B[(t) 	% 2][x + STRIDE * 0][y + VVECLEN - 1] = tmp[7];
										B[(t+1) % 2][x + STRIDE * 1][y + VVECLEN    ] = tmp[6];
										B[(t) 	% 2][x + STRIDE * 2][y + VVECLEN + 1] = tmp[5];
										B[(t+1) % 2][x + STRIDE * 3][y + VVECLEN + 2] = tmp[4];
										B[(t) 	% 2][x + STRIDE * 4][y + VVECLEN + 3] = tmp[3];
										B[(t+1) % 2][x + STRIDE * 5][y + VVECLEN + 4] = tmp[2];
										B[(t) 	% 2][x + STRIDE * 6][y + VVECLEN + 5] = tmp[1];
										B[(t+1) % 2][x + STRIDE * 7][y + VVECLEN + 6] = tmp[0];
									}
						    		Input_Output_8_512(out, v_center_7, in);
									
						    		vstore_512_blk(B[(t) % 2][x][y], out);
						    	}
								if((y >= myybeg - VVECLEN + 1 + myyb)){
									_mm512_storeu_pd(tmp, v_x_minus_0);
										
									B[(t ) % 2][x-1 + STRIDE * 7][y+6] = tmp[0];
									B[(t + 1) % 2][x-1 + STRIDE * 6][y+5] = tmp[1];
									B[(t) % 2][x-1 + STRIDE * 5][y+4] = tmp[2];
									B[(t + 1) % 2][x-1 + STRIDE * 4][y+3] = tmp[3];
									B[(t) % 2][x-1 + STRIDE * 3][y+2] = tmp[4];
									B[(t + 1) % 2][x-1 + STRIDE * 2][y+1] = tmp[5];
									B[(t) % 2][x-1 + STRIDE * 1][y] = tmp[6];
									B[(t + 1) % 2][x-1][y-1] = tmp[7];
								}
						    	for ( ; y < myybeg - VVECLEN + 1 + myyb; y++) {

						    		v_x_minus_2 = _mm512_set_pd(B[(t + 1) % 2][x - 1][y + 1], \
                                                                B[(t) % 2][x - 1 + STRIDE][y+1 + 1], \
                                                                B[(t + 1) % 2][x - 1 + STRIDE * 2][y+2 + 1], \
                                                                B[(t) % 2][x - 1 + STRIDE * 3][y+3 + 1], \
                                                                B[(t + 1) % 2][x - 1 + STRIDE * 4][y+4 + 1], \
                                                                B[(t) % 2][x - 1 + STRIDE * 5][y+5 + 1], \
                                                                B[(t + 1) % 2][x - 1 + STRIDE * 6][y+6 + 1], \
                                                                B[(t) % 2][x - 1 + STRIDE * 7][y+7 + 1]);

						    		v_x_plus_2  = _mm512_set_pd(B[(t + 1) % 2][x + 1][y + 1], \
                                                                B[(t) % 2][x + 1 + STRIDE][y+1 + 1], \
                                                                B[(t + 1) % 2][x + 1 + STRIDE * 2][y+2 + 1], \
                                                                B[(t) % 2][x + 1 + STRIDE * 3][y+3 + 1], \
                                                                B[(t + 1) % 2][x + 1 + STRIDE * 4][y+4 + 1], \
                                                                B[(t) % 2][x + 1 + STRIDE * 5][y+5 + 1], \
                                                                B[(t + 1) % 2][x + 1 + STRIDE * 6][y+6 + 1], \
                                                                B[(t) % 2][x + 1 + STRIDE * 7][y+7 + 1]);

						    		v_center_2  = _mm512_set_pd(B[(t + 1) % 2][x][y + 1], \
                                                                B[(t) % 2][x + STRIDE][y + 1 +1], \
                                                                B[(t + 1) % 2][x + STRIDE * 2][y + 1 +2], \
                                                                B[(t) % 2][x + STRIDE * 3][y + 1 +3], \
                                                                B[(t + 1) % 2][x + STRIDE * 4][y + 1 +4], \
                                                                B[(t) % 2][x + STRIDE * 5][y + 1 +5], \
                                                                B[(t + 1) % 2][x + STRIDE * 6][y + 1 +6], \
                                                                B[(t) % 2][x + STRIDE * 7][y + 1 +7]);

									Compute_1vector_512(v_x_minus_2, v_center_2, v_x_plus_2,\
															v_x_minus_1, v_center_1, v_x_plus_1,\
															v_x_minus_0, v_center_0, v_x_plus_0);

						    		_mm512_storeu_pd(tmp, v_center_0);
									
						    		B[(t + 1) % 2][x + STRIDE * 7][y+7] = tmp[0];
						    		B[t % 2][x + STRIDE * 6][y+6] = tmp[1];
						    		B[(t + 1) % 2][x + STRIDE * 5][y+5] = tmp[2];
						    		B[t % 2][x + STRIDE * 4][y+4] = tmp[3];
						    		B[(t + 1) % 2][x + STRIDE * 3][y+3] = tmp[4];
						    		B[t % 2][x + STRIDE * 2][y+2] = tmp[5];
						    		B[(t + 1) % 2][x + STRIDE * 1][y+1] = tmp[6];
						    		B[t % 2][x][y] = tmp[7];

						    		v_center_0 = v_center_1;
						    		v_center_1 = v_center_2;
						    		v_x_plus_0 = v_x_plus_1;
						    		v_x_plus_1 = v_x_plus_2;
						    		v_x_minus_0 = v_x_minus_1;
						    		v_x_minus_1 = v_x_minus_2;
						    	}

						    }
						    for (; x < xmax - STRIDE * VVECLEN + 2; x++) {

						    	for (y = myybeg - VVECLEN + 1; y <= myybeg - VVECLEN + 1 + myyb - VVECLEN; y += VVECLEN) {

						    		vload_512_blk(v_center_0, B[(t + 1) % 2][x - XSLOPE][y]);
						    		vload_512_blk(v_center_1, B[(t) % 2][x - XSLOPE + STRIDE][y+1]);
						    		vload_512_blk(v_center_2, B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y+2]);
						    		vload_512_blk(v_center_3, B[(t) % 2][x - XSLOPE + STRIDE * 3][y+3]);
						    		vload_512_blk(v_center_4, B[(t + 1) % 2][x - XSLOPE + STRIDE * 4][y+4]);
						    		vload_512_blk(v_center_5, B[(t) % 2][x - XSLOPE + STRIDE * 5][y+5]);
						    		vload_512_blk(v_center_6, B[(t + 1) % 2][x - XSLOPE + STRIDE * 6][y+6]);
						    		vload_512_blk(v_center_7, B[(t) % 2][x - XSLOPE + STRIDE * 7][y+7]);

						    		transpose_512(v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7, in, out);

						    		vstore_512_blk(B[(t + 1) % 2][x - XSLOPE + STRIDE * 0][y], v_center_7);
						    		vstore_512_blk(B[(t) % 2][x - XSLOPE + STRIDE * 1][y+1], v_center_6);
						    		vstore_512_blk(B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y+2], v_center_5);
						    		vstore_512_blk(B[(t) % 2][x - XSLOPE + STRIDE * 3][y+3], v_center_4);
						    		vstore_512_blk(B[(t + 1) % 2][x - XSLOPE + STRIDE * 4][y+4], v_center_3);
						    		vstore_512_blk(B[(t) % 2][x - XSLOPE + STRIDE * 5][y+5], v_center_2);
						    		vstore_512_blk(B[(t + 1) % 2][x - XSLOPE + STRIDE * 6][y+6], v_center_1);
						    		vstore_512_blk(B[(t) % 2][x - XSLOPE + STRIDE * 7][y+7], v_center_0);
						    	}
						    }

                            // tail
						    xmin = xmax - STRIDE * VVECLEN - 1;
						    for (t = tv; t < tv + VVECLEN; t++) {

								xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel] + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
						    	
								for (x = xmin + STRIDE * (VVECLEN - 1 - (t - tv)); x < xmax; x++) {

						    		#pragma ivdep
						    		#pragma vector always
						    		// AVX-256 9-point hybrid: N,E,W,S,NW,NE,SW,SE order
					__m256d vc0 = _mm256_set1_pd(C0);
					__m256d vc1 = _mm256_set1_pd(C1);
					for (y = myybeg - (t - tv); y + 3 < myybeg - (t - tv) + myyb; y += 4){
						__m256d vn  = _mm256_loadu_pd(&B[t%2][x][y-1]);
						__m256d ve  = _mm256_loadu_pd(&B[t%2][x+1][y]);
						__m256d vw  = _mm256_loadu_pd(&B[t%2][x-1][y]);
						__m256d vs  = _mm256_loadu_pd(&B[t%2][x][y+1]);
						__m256d vnw = _mm256_loadu_pd(&B[t%2][x-1][y-1]);
						__m256d vne = _mm256_loadu_pd(&B[t%2][x+1][y-1]);
						__m256d vsw = _mm256_loadu_pd(&B[t%2][x-1][y+1]);
						__m256d vse = _mm256_loadu_pd(&B[t%2][x+1][y+1]);
						__m256d vm  = _mm256_loadu_pd(&B[t%2][x][y]);
						__m256d stub = _mm256_add_pd(vn, ve);
						stub = _mm256_add_pd(stub, vw);
						stub = _mm256_add_pd(stub, vs);
						stub = _mm256_add_pd(stub, vnw);
						stub = _mm256_add_pd(stub, vne);
						stub = _mm256_add_pd(stub, vsw);
						stub = _mm256_add_pd(stub, vse);
						__m256d res = _mm256_fmadd_pd(vc0, vm, _mm256_mul_pd(vc1, stub));
						_mm256_storeu_pd(&B[(t+1)%2][x][y], res);
					}
					for (; y < myybeg - (t - tv) + myyb; y++){
						Compute_scalar(B, t, x, y);
					}
						    	}
						    }

							for (t = tv; t < tv + VVECLEN; t++){

						    	xmin = (mylevel == 1 && xx == 0) ?	             XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
						        xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
								
						    	for (x = xmin; x < xmax; x++) {

						    		#pragma ivdep
									#pragma vector always
									for (y = myybeg + myyb - (t - tv); y < min(NY + YSTART, ybeg + yb - (t - tv)); y++) {
						    			Compute_scalar(B, t, x, y);

						    		}
						    	}
						    }
                        }
					}

					//extra
					for (t = tv; t < min(tt + 2 * tb, T); t++) {
						
						xmin = (mylevel == 1 && xx == 0) ?	             XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
						xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
						
						for (x = xmin; x < xmax; x++) {

							#pragma ivdep
							#pragma vector always
							for (y = max(YSTART, ybeg - (t - tv)); y < min(NY + YSTART, ybeg - (t - tv) + yb); y++) {
								Compute_scalar(B, t, x, y);
							}
						}
					}
				}
			}
		}
	}
}
