#include "define.h"

void blocking_parallel_rectangle_vectime(double *A, int NX, int NY, int T, int xb, int yb, int tb)
{

	double(*B)[NX + 2 * XSTART][NY + 2 * YSTART] = (double(*)[NX + 2 * XSTART][NY + 2 * YSTART]) A;

	int tt, t, tv, x, xx, y, yy;

	tb -= tb % 4;
	int Bx = xb;
	int bx = Bx - 2 * tb * XSLOPE;
	int ix = Bx + bx;
	int nb0[2] = {myfloor(NX - Bx, ix), myfloor(NX - Bx, ix) + 1};
	int nrestpoints = NX % ix;
	int bx_first_B1 = (Bx + nrestpoints) / 2;
	int bx_last_B1 = (Bx + nrestpoints) - bx_first_B1;
	int xright[2] = {bx_first_B1 + Bx + XSTART, bx_first_B1 + (Bx - bx) / 2 + XSTART};
    // printf("Bx:%d bx: %d ix:%d nb0:%d %d\nnrest:%d bxfirst:%d xright:%d %d\n",    \
    //         Bx,   bx,    ix,  nb0[0],nb0[1],nrestpoints,bx_first_B1,xright[0],xright[1]);

	int wave;
	int mylevel;
	int xmin, xmax, ybeg;
    int myybeg, myyb;

	for (wave = 0; wave < myceil(T, tb) + 1 + myceil(NY + T - 1, yb); wave++) { 
        // printf("## wave:%d\n", wave);
		#pragma omp parallel for private(mylevel, tt, tv, t, x, y, xmin, xmax, ybeg, myybeg, myyb)  collapse(2) schedule(dynamic, 1)

		for (xx = 0; xx < max(nb0[0], nb0[1]); xx++) {

			for (yy = 0; yy < 2 * myceil(NY + T - 1, yb); yy++) {

				vec v_x_plus_0, v_x_plus_1, v_x_plus_2, v_x_plus_3;
				vec v_x_minus_0, v_x_minus_1, v_x_minus_2, v_x_minus_3;
				vec v_center_0, v_center_1, v_center_2, v_center_3;
				vec in, out;
				SET_COFF;
				double tmp[4];

				mylevel = (wave % 2 + yy) % 2;
                
				if (xx < nb0[mylevel]) {

					tt = -tb + (wave - yy) * tb;

                    ybeg = YSTART - wave * tb + yy * (yb + tb);
					
					for (tv = max(tt, 0); tv <= min(tt + 2 * tb, T) - VECLEN; tv += VECLEN, ybeg -= VECLEN) {
						
                        if(ybeg - VECLEN + 1 < YSTART){
					        myybeg 	= 	YSTART + VECLEN - 1;
					        myyb	=	yb - (myybeg - ybeg);
					    } else {
					    	myybeg	=	ybeg;
					    	myyb	=	yb;
					    }
					    if (ybeg + yb >= YSTART + NY){	
					    	myyb -= ybeg + yb - (YSTART + NY);
					    }
                        if (myyb <= 2 * VECLEN ) {
						    for (t = tv; t < tv + VECLEN; t++){
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
						    //head
							for (t = tv; t < tv + VECLEN - 1; t++){

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

						    for (t = tv; t < tv + VECLEN; t++) {

						    	xmin = (mylevel == 1 && xx == 0) ? XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
						    	xmax = xmin + STRIDE * (VECLEN - 1 - (t - tv));
						    	if (!(mylevel == 1 && xx == 0)) {
						    		xmax += ((tt + tb < tv + 1) ? 1 : -1) * (VECLEN - 1 - (t - tv));
						    	}
						    	for (x = xmin; x < xmax + 2; x++) {

						    		#pragma vector always
						    		#pragma ivdep
						    		for (y = myybeg - (t - tv); y < myybeg - (t - tv) + myyb ; y++) {
						    			Compute_scalar(B, t, x, y);
						    		}
						    	}
						    }

						    t = tv;

						    xmin += 1;
						    xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel] + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
						    
							for (x = xmin; x < xmin + STRIDE + 1; x++) {

						    	for (y = myybeg - VECLEN + 1; y <= myybeg - VECLEN + 1 + myyb - VECLEN; y += VECLEN) {

						    		vload(v_center_3, B[(t + 1) % 2][x][y]);
						    		vload(v_center_2, B[(t) % 2][x + STRIDE][y+1]);
						    		vload(v_center_1, B[(t + 1) % 2][x + STRIDE * 2][y+2]);
						    		vload(v_center_0, B[(t) % 2][x + STRIDE * 3][y+3]);

						    		transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);

						    		vstore(B[(t + 1) % 2][x + STRIDE * 0][y], v_center_0);
						    		vstore(B[(t) % 2][x + STRIDE * 1][y+1], v_center_1);
						    		vstore(B[(t + 1) % 2][x + STRIDE * 2][y+2], v_center_2);
						    		vstore(B[(t) % 2][x + STRIDE * 3][y+3], v_center_3);
						    	}
						    }
						    for (x = xmin + XSLOPE; x < xmax - STRIDE * VECLEN - 1; x++) {

						    	y = myybeg - VECLEN + 1;

						    	v_center_0 = _mm256_set_pd(B[(t + 1) % 2][x             ][y - 1],\
                                                           B[(t)     % 2][x + STRIDE    ][y    ],\
                                                           B[(t + 1) % 2][x + STRIDE * 2][y + 1], \
                                                           B[(t)     % 2][x + STRIDE * 3][y + 2]);
						    	v_x_plus_0 = _mm256_set_pd(B[(t + 1) % 2][x + 1            ][y - 1],\
                                                           B[(t)     % 2][x + 1 + STRIDE    ][y    ],\
                                                           B[(t + 1) % 2][x + 1 + STRIDE * 2][y + 1], \
                                                           B[(t)     % 2][x + 1 + STRIDE * 3][y + 2]);
						    	v_x_minus_0 = _mm256_set_pd(B[(t + 1) % 2][x - 1             ][y - 1],\
                                                           B[(t)     % 2][x - 1 + STRIDE    ][y    ],\
                                                           B[(t + 1) % 2][x - 1 + STRIDE * 2][y + 1], \
                                                           B[(t)     % 2][x - 1 + STRIDE * 3][y + 2]);

						    	v_center_3    = _mm256_set_pd(B[(t + 1) % 2][x + STRIDE    ][y    ],\
                                                           B[(t)     % 2][x + STRIDE * 2][y + 1],\
                                                           B[(t + 1) % 2][x + STRIDE * 3][y + 2], \
                                                           B[(t)     % 2][x + STRIDE * 4][y + 3]);

						    	vload(v_center_1, B[(t + 1) % 2][x][y]);
						    	vload(v_x_minus_1, B[(t + 1) % 2][x - XSLOPE][y]);
						    	vload(v_x_plus_1, B[(t + 1) % 2][x + XSLOPE][y]);

						    	for (; y <= myybeg - VECLEN + 1 + myyb - VECLEN; y += VECLEN) {

						    		vload(in, B[(t) % 2][x + STRIDE * 4][y + VECLEN]); // the next x iter in vector

									vstore(B[(t + 1) % 2][x + STRIDE][y], v_center_3);

						    		vload(v_x_minus_2, B[(t) % 2][x - XSLOPE + STRIDE][y+1]);
						    		vload(v_x_plus_2, B[(t) % 2][x + XSLOPE + STRIDE][y+1]);
						    		vload(v_center_2, B[(t) % 2][x + STRIDE][y+1]);
									Compute_1vector(v_x_minus_2, v_center_2, v_x_plus_2,\
													v_x_minus_1, v_center_1, v_x_plus_1,\
													v_x_minus_0, v_center_0, v_x_plus_0);
						    		Input_Output_1(out, v_center_0, in);
									vstore(B[(t) % 2][x + STRIDE * 2][y+1], v_center_0);

						    		vload(v_x_minus_3, B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y+2]);
						    		vload(v_x_plus_3, B[(t + 1) % 2][x + XSLOPE + STRIDE * 2][y+2]);
						    		vload(v_center_3, B[(t + 1) % 2][x + STRIDE * 2][y+2]);
									Compute_1vector(v_x_minus_3, v_center_3, v_x_plus_3,\
													v_x_minus_2, v_center_2, v_x_plus_2,\
													v_x_minus_1, v_center_1, v_x_plus_1);
						    		Input_Output_2(out, v_center_1, in);
									vstore(B[(t + 1) % 2][x + STRIDE * 3][y+2], v_center_1);

						    		vload(v_x_minus_0, B[(t) % 2][x - XSLOPE + STRIDE * 3][y+3]);
						    		vload(v_x_plus_0, B[(t) % 2][x + XSLOPE + STRIDE * 3][y+3]);
						    		vload(v_center_0, B[(t) % 2][x + STRIDE * 3][y+3]);
									Compute_1vector(v_x_minus_0, v_center_0, v_x_plus_0,\
													v_x_minus_3, v_center_3, v_x_plus_3,\
													v_x_minus_2, v_center_2, v_x_plus_2);
						    		Input_Output_3(out, v_center_2, in);
									vstore(B[(t) % 2][x + STRIDE * 4][y+3], v_center_2);


						    		 v_center_1 = (y > myybeg - VECLEN + 1 + myyb - VECLEN -VECLEN) ?  \
                                                    (_mm256_set_pd(	B[(t + 1) % 2][x][y + VECLEN], \
																	B[(t) % 2][x + STRIDE][y + VECLEN + 1], \
						    										B[(t + 1) % 2][x + STRIDE * 2][y + VECLEN + 2], \
																	B[(t) % 2][x + STRIDE * 3][y + VECLEN + 3])) \
						    						: _mm256_loadu_pd(&B[(t + 1) % 2][x][y + VECLEN]);
						    		 v_x_plus_1 = (y > myybeg - VECLEN + 1 + myyb - VECLEN -VECLEN) ?  \
                                                    (_mm256_set_pd(	B[(t + 1) % 2][x + XSLOPE][y + VECLEN],\
																	B[(t) % 2][x + XSLOPE + STRIDE][y + VECLEN + 1], \
						    										B[(t + 1) % 2][x + XSLOPE + STRIDE * 2][y + VECLEN + 2], \
																	B[(t) % 2][x + XSLOPE + STRIDE * 3][y + VECLEN + 3])) \
						    						: _mm256_loadu_pd(&B[(t + 1) % 2][x + XSLOPE][y + VECLEN]);
						    		 v_x_minus_1 = (y > myybeg - VECLEN + 1 + myyb - VECLEN -VECLEN) ?  \
                                                    (_mm256_set_pd(	B[(t + 1) % 2][x - XSLOPE][y + VECLEN], \
																	B[(t) % 2][x - XSLOPE + STRIDE][y + VECLEN + 1], \
						    										B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y + VECLEN + 2], 
																	B[(t) % 2][x - XSLOPE + STRIDE * 3][y + VECLEN + 3])) \
						    						: _mm256_loadu_pd(&B[(t + 1) % 2][x - XSLOPE][y + VECLEN]);
									Compute_1vector(v_x_minus_1, v_center_1, v_x_plus_1,\
													v_x_minus_0, v_center_0, v_x_plus_0,\
													v_x_minus_3, v_center_3, v_x_plus_3);
									
									if (y >  myybeg - VECLEN + 1 + myyb - VECLEN -VECLEN) {
										_mm256_storeu_pd(tmp, v_center_3);
										B[(t) 	% 2][x + STRIDE * 0][y + VECLEN - 1] = tmp[3];
										B[(t+1) % 2][x + STRIDE * 1][y + VECLEN    ] = tmp[2];
										B[(t) 	% 2][x + STRIDE * 2][y + VECLEN + 1] = tmp[1];
										B[(t+1) % 2][x + STRIDE * 3][y + VECLEN + 2] = tmp[0];
									}
						    		Input_Output_4(out, v_center_3, in);
									
						    		vstore(B[(t) % 2][x][y], out);
						    	}
								if((y >= myybeg - VECLEN + 1 + myyb)){
									_mm256_storeu_pd(tmp, v_x_minus_0);
										
									B[(t ) % 2][x-1 + STRIDE * 3][y+2] = tmp[0];
									B[(t + 1) % 2][x-1 + STRIDE * 2][y+1] = tmp[1];
									B[(t) % 2][x-1 + STRIDE * 1][y] = tmp[2];
									B[(t + 1) % 2][x-1][y-1] = tmp[3];
								}
						    	for ( ; y < myybeg - VECLEN + 1 + myyb; y++) {

						    		v_x_minus_2 = _mm256_set_pd(B[(t + 1) % 2][x - 1][y + 1], \
                                                                B[(t) % 2][x - 1 + STRIDE][y+1 + 1], \
                                                                B[(t + 1) % 2][x - 1 + STRIDE * 2][y+2 + 1], \
                                                                B[(t) % 2][x - 1 + STRIDE * 3][y+3 + 1]);

						    		v_x_plus_2  = _mm256_set_pd(B[(t + 1) % 2][x + 1][y + 1], \
                                                                B[(t) % 2][x + 1 + STRIDE][y+1 + 1], \
                                                                B[(t + 1) % 2][x + 1 + STRIDE * 2][y+2 + 1], \
                                                                B[(t) % 2][x + 1 + STRIDE * 3][y+3 + 1]);

						    		v_center_2  = _mm256_set_pd(B[(t + 1) % 2][x][y + 1], \
                                                                B[(t) % 2][x + STRIDE][y + 1 +1], \
                                                                B[(t + 1) % 2][x + STRIDE * 2][y + 1 +2], \
                                                                B[(t) % 2][x + STRIDE * 3][y + 1 +3]);

									Compute_1vector(v_x_minus_2, v_center_2, v_x_plus_2,\
													v_x_minus_1, v_center_1, v_x_plus_1,\
													v_x_minus_0, v_center_0, v_x_plus_0);

						    		_mm256_storeu_pd(tmp, v_center_0);
									
						    		B[(t + 1) % 2][x + STRIDE * 3][y+3] = tmp[0];
						    		B[t % 2][x + STRIDE * 2][y+2] = tmp[1];
						    		B[(t + 1) % 2][x + STRIDE * 1][y+1] = tmp[2];
						    		B[t % 2][x][y] = tmp[3];

						    		v_center_0 = v_center_1;
						    		v_center_1 = v_center_2;
						    		v_x_plus_0 = v_x_plus_1;
						    		v_x_plus_1 = v_x_plus_2;
						    		v_x_minus_0 = v_x_minus_1;
						    		v_x_minus_1 = v_x_minus_2;
						    	}

						    }
						    for (; x < xmax - STRIDE * VECLEN + 2; x++) {

						    	for (y = myybeg - VECLEN + 1; y <= myybeg - VECLEN + 1 + myyb - VECLEN; y += VECLEN) {

						    		vload(v_center_0, B[(t + 1) % 2][x - XSLOPE][y]);
						    		vload(v_center_1, B[(t) % 2][x - XSLOPE + STRIDE][y+1]);
						    		vload(v_center_2, B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y+2]);
						    		vload(v_center_3, B[(t) % 2][x - XSLOPE + STRIDE * 3][y+3]);

						    		transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);

						    		vstore(B[(t + 1) % 2][x - XSLOPE + STRIDE * 0][y], v_center_3);
						    		vstore(B[(t) % 2][x - XSLOPE + STRIDE * 1][y+1], v_center_2);
						    		vstore(B[(t + 1) % 2][x - XSLOPE + STRIDE * 2][y+2], v_center_1);
						    		vstore(B[(t) % 2][x - XSLOPE + STRIDE * 3][y+3], v_center_0);
						    	}
						    }

                            // tail
						    xmin = xmax - STRIDE * VECLEN - 1;
						    for (t = tv; t < tv + VECLEN; t++) {

								xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel] + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);
						    	
								for (x = xmin + STRIDE * (VECLEN - 1 - (t - tv)); x < xmax; x++) {

						    		#pragma ivdep
						    		#pragma vector always
						    		for (y = myybeg - (t - tv); y < myybeg - (t - tv) + myyb; y++) {
						    			Compute_scalar(B, t, x, y);
						    		}
						    	}
						    }

							for (t = tv; t < tv + VECLEN; t++){

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
