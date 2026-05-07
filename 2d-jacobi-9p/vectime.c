#include "define.h"

void vectime(double* A, int NX, int NY, int T) {
	double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;
    double tmp[4];

    int tt, t = 0, x, xx, y;
    __m256d v_center_0, v_center_1, v_center_2, v_center_3; 
    __m256d v_x_plus_0, v_x_plus_1, v_x_plus_2, v_x_plus_3; 
    __m256d v_x_minus_0, v_x_minus_1, v_x_minus_2, v_x_minus_3;
    __m256d in, out;
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

        for(x = XSTART - XSLOPE; x <= XSTART +XSLOPE; x++){
			for ( y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN ){
				vload(v_center_0, B[t%2    ][x + STRIDE *3][y]);
				vload(v_center_1, B[(t+1)%2][x + STRIDE *2][y]);
				vload(v_center_2, B[t%2    ][x + STRIDE *1][y]);
				vload(v_center_3, B[(t+1)%2][x            ][y]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore( B[t%2    ][x + STRIDE *3][y], v_center_3);
				vstore( B[(t+1)%2][x + STRIDE *2][y], v_center_2);
				vstore( B[t%2    ][x + STRIDE *1][y], v_center_1);
				vstore( B[(t+1)%2][x            ][y], v_center_0);
			}
		}        
                
        for ( x = XSTART ; x <= NX + XSTART - STRIDE * VECLEN ; x ++){
            y = YSTART;
            vloadset(v_x_minus_0, B[(t+1)%2][x-XSLOPE         ][y-YSLOPE], \
                          B[(t)%2  ][x-XSLOPE+STRIDE  ][y-YSLOPE], \
                          B[(t+1)%2][x-XSLOPE+STRIDE*2][y-YSLOPE], \
                          B[(t)%2  ][x-XSLOPE+STRIDE*3][y-YSLOPE]);

            vloadset(v_center_0, B[(t+1)%2][x         ][y-YSLOPE], \
                          B[(t)%2  ][x+STRIDE  ][y-YSLOPE], \
                          B[(t+1)%2][x+STRIDE*2][y-YSLOPE], \
                          B[(t)%2  ][x+STRIDE*3][y-YSLOPE]);

            vloadset(v_x_plus_0, B[(t+1)%2][x+XSLOPE         ][y-YSLOPE], \
                          B[(t)%2  ][x+XSLOPE+STRIDE  ][y-YSLOPE], \
                          B[(t+1)%2][x+XSLOPE+STRIDE*2][y-YSLOPE], \
                          B[(t)%2  ][x+XSLOPE+STRIDE*3][y-YSLOPE]);

            vload(v_x_minus_1, B[(t+1)%2  ][x-XSLOPE][y]);
            vload(v_center_1, B[(t+1)%2  ][x       ][y]);
            vload(v_x_plus_1, B[(t+1)%2  ][x+XSLOPE][y]);

            for ( y = YSTART; y <= NY + YSTART - VECLEN; y+= VECLEN) {
                vload(v_x_minus_3, B[(t+1)%2][x-XSLOPE+STRIDE*2][y]);
                vload(v_x_minus_2, B[(t)%2  ][x-XSLOPE+STRIDE  ][y]);         //(x, y) = (-1, -1) left

                vload(v_center_3, B[(t+1)%2][x+STRIDE*2][y]);
                vload(v_center_2, B[(t)%2  ][x+STRIDE  ][y]);                //(x, y) = (0, -1) mid 

                vload(v_x_plus_3, B[(t+1)%2][x+XSLOPE+STRIDE*2][y]);
                vload(v_x_plus_2, B[(t)%2  ][x+XSLOPE+STRIDE  ][y]);         //(x, y) = (1, -1) right

                vload(in, B[(t)%2][x+STRIDE*4][y]);   // the next x iter in vector

                Compute_1vector(v_x_minus_2, v_center_2, v_x_plus_2,\
                                v_x_minus_1, v_center_1, v_x_plus_1,\
                                v_x_minus_0, v_center_0, v_x_plus_0);   //1st   store the newest value to the left vec
                v_x_minus_0 = vrotate_high2low(v_x_minus_0);
                out =v_x_minus_0;
                v_x_minus_0 = _mm256_blend_pd(v_x_minus_0, in, 0b0001);
                vstore(B[(t+1)%2  ][x+STRIDE][y], v_x_minus_0);   //for the compute of next x iteration


                Compute_1vector(v_x_minus_3, v_center_3, v_x_plus_3,\
                                v_x_minus_2, v_center_2, v_x_plus_2,\
                                v_x_minus_1, v_center_1, v_x_plus_1); 
                in = _mm256_permute_pd(in, 0b0101);
                v_x_minus_1 = vrotate_high2low(v_x_minus_1);
                out = _mm256_shuffle_pd(out, v_x_minus_1, 0b0000);
                v_x_minus_1 = _mm256_blend_pd(v_x_minus_1, in, 0b0001);
                vstore(B[(t)%2][x+STRIDE*2][y], v_x_minus_1);  //for the compute of next x iteration


                vload(v_x_minus_0, B[(t)%2][x-XSLOPE+STRIDE*3][y]);
                vload(v_center_0, B[(t)%2][x       +STRIDE*3][y]);
                vload(v_x_plus_0, B[(t)%2][x+XSLOPE+STRIDE*3][y]);
                Compute_1vector(v_x_minus_0, v_center_0, v_x_plus_0,\
                                v_x_minus_3, v_center_3, v_x_plus_3,\
                                v_x_minus_2, v_center_2, v_x_plus_2); 
                out = _mm256_blend_pd(out, v_x_minus_2, 0b1000);
                v_x_minus_2 = _mm256_blend_pd(v_x_minus_2, in, 0b1000);
                v_x_minus_2 = vrotate_high2low(v_x_minus_2);	
                vstore(B[(t+1)%2  ][x+STRIDE*3][y], v_x_minus_2);

                out = _mm256_permute_pd(out, 0b0110);
                in = _mm256_permute_pd(in, 0b0101);


                if(y + VECLEN <= NY + YSTART - VECLEN){
                    vload(v_x_minus_1, B[(t+1)%2][x-XSLOPE][y+VECLEN]);
                    vload(v_center_1, B[(t+1)%2][x       ][y+VECLEN]);
                    vload(v_x_plus_1, B[(t+1)%2][x+XSLOPE][y+VECLEN]);

                } else {

                    vloadset(v_x_minus_1, B[(t+1)%2][x-XSLOPE         ][y+VECLEN], \
                                B[(t)%2  ][x-XSLOPE+STRIDE  ][y+VECLEN], \
                                B[(t+1)%2][x-XSLOPE+STRIDE*2][y+VECLEN], \
                                B[(t)%2  ][x-XSLOPE+STRIDE*3][y+VECLEN]);

                    vloadset(v_center_1, B[(t+1)%2][x         ][y+VECLEN], \
                                B[(t)%2  ][x+STRIDE  ][y+VECLEN], \
                                B[(t+1)%2][x+STRIDE*2][y+VECLEN], \
                                B[(t)%2  ][x+STRIDE*3][y+VECLEN]);

                    vloadset(v_x_plus_1, B[(t+1)%2][x+XSLOPE         ][y+VECLEN], \
                                B[(t)%2  ][x+XSLOPE+STRIDE  ][y+VECLEN], \
                                B[(t+1)%2][x+XSLOPE+STRIDE*2][y+VECLEN], \
                                B[(t)%2  ][x+XSLOPE+STRIDE*3][y+VECLEN]);
                }
                Compute_1vector(v_x_minus_1, v_center_1, v_x_plus_1,\
                                v_x_minus_0, v_center_0, v_x_plus_0,\
                                v_x_minus_3, v_center_3, v_x_plus_3); 
                out = _mm256_blend_pd(out, v_x_minus_3, 0b1000);
                v_x_minus_3 = _mm256_blend_pd(v_x_minus_3, in, 0b1000);
                v_x_minus_3 = vrotate_high2low(v_x_minus_3);
                vstore(B[(t)%2][x+STRIDE*4][y], v_x_minus_3);

                vstore(B[(t)%2][x][y], out);

            }
         
            for ( y += 1; y <= NY+YSTART; y++){
                vloadset(v_x_minus_2, B[(t+1)%2][x-XSLOPE         ][y], \
                              B[(t)%2  ][x-XSLOPE+STRIDE  ][y], \
                              B[(t+1)%2][x-XSLOPE+STRIDE*2][y], \
                              B[(t)%2  ][x-XSLOPE+STRIDE*3][y]);

                vloadset(v_center_2, B[(t+1)%2][x         ][y], \
                              B[(t)%2  ][x+STRIDE  ][y], \
                              B[(t+1)%2][x+STRIDE*2][y], \
                              B[(t)%2  ][x+STRIDE*3][y]);

                vloadset(v_x_plus_2, B[(t+1)%2][x+XSLOPE         ][y], \
                              B[(t)%2  ][x+XSLOPE+STRIDE  ][y], \
                              B[(t+1)%2][x+XSLOPE+STRIDE*2][y], \
                              B[(t)%2  ][x+XSLOPE+STRIDE*3][y]);

                Compute_1vector(v_x_minus_2, v_center_2, v_x_plus_2,\
                                v_x_minus_1, v_center_1, v_x_plus_1,\
                                v_x_minus_0, v_center_0, v_x_plus_0); 
                _mm256_storeu_pd(tmp, v_x_minus_0);

		    	B[(t+1)%2][x + STRIDE * 3][y - 1] = tmp[0];
		    	B[t%2    ][x + STRIDE * 2][y - 1] = tmp[1];
		    	B[(t+1)%2][x + STRIDE * 1][y - 1] = tmp[2];
		    	B[t%2    ][x             ][y - 1] = tmp[3];

                v_x_minus_0 = v_x_minus_1;
                v_center_0 = v_center_1;
                v_x_plus_0 = v_x_plus_1;
                v_x_minus_1 = v_x_minus_2;
                v_center_1 = v_center_2;
                v_x_plus_1 = v_x_plus_2;
                
            }
        }

        xx = x;
        for(x = xx - XSLOPE; x <= xx +XSLOPE; x++){
			for ( y = YSTART; y <= NY + YSTART - VECLEN; y += VECLEN ){
				vload(v_center_3, B[t%2    ][x + STRIDE *3][y]);
				vload(v_center_2, B[(t+1)%2][x + STRIDE *2][y]);
				vload(v_center_1, B[t%2    ][x + STRIDE *1][y]);
				vload(v_center_0, B[(t+1)%2][x            ][y]);
				transpose(v_center_0, v_center_1, v_center_2, v_center_3, in, out);
				vstore( B[t%2    ][x + STRIDE *3][y], v_center_0);
				vstore( B[(t+1)%2][x + STRIDE *2][y], v_center_1);
				vstore( B[t%2    ][x + STRIDE *1][y], v_center_2);
				vstore( B[(t+1)%2][x            ][y], v_center_3);
			}
		} 
    
        for( t = tt ; t < tt + VECLEN ; t++){
			for ( x = xx + STRIDE * (VECLEN - 1 - (t - tt)); x < NX + XSTART; x++) {
                #pragma ivdep
                #pragma vector always
                for ( y = YSTART; y < NY + YSTART; y++) {
                    Compute_scalar(B, t, x, y);
                }		
            }
		}
        for ( y = 0; y < NY + YSTART * 2 ; y++) {
            B[1][XSTART - XSLOPE][y] = B[0][XSTART - XSLOPE][y];
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
