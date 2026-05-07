#include "define.h"

void vectime_fixed_array(double* A, int NX, int NY, int T) {
	long int i, j;

    double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART]) A;

    double tmp[4];

    int tt, t =0, x, xx, y, yy;
    __m256d v00, v10, v20, v30, v01, v11, v21, v31, v02, v12, v22, v32;
    __m256d in, out;
	SET_COFF;   //only use c1


	for ( tt = 0; tt <= T - VECLEN; tt += VECLEN){	
		for( t = tt ; t < tt + VECLEN ; t++){		//head
			for ( x = XSTART; x < XSTART + STRIDE * (VECLEN - 1 - (t - tt)) + 1; x++) {//ASSERT VECLEN <= STRIDE + 1
            #pragma ivdep
            #pragma vector always
                for ( y = YSTART; y < NY + YSTART; y++) {
                    Compute_scalar(B, t, x, y);
                }		
            }
		}
        // if(t == 3) printf("In kernel(B):t=2 %f %f %f %f %f %f %f %f %f\n",
        //                   B[(t)%2][VECLEN-1][VECLEN-1], B[(t)%2][VECLEN][VECLEN-1], B[(t)%2][VECLEN+1][VECLEN-1],\
        //                   B[(t)%2][VECLEN-1][VECLEN  ], B[(t)%2][VECLEN][VECLEN  ], B[(t)%2][VECLEN+1][VECLEN  ],\
        //                   B[(t)%2][VECLEN-1][VECLEN+1], B[(t)%2][VECLEN][VECLEN+1], B[(t)%2][VECLEN+1][VECLEN+1]\
        //                   );
        
        t = tt;
        x = XSTART + 1;
		for ( y = YSTART; y <= NY + YSTART - VECLEN ; y += VECLEN ){
			vload(v00, B[t%2    ][x - XSLOPE + STRIDE *3][y]);
			vload(v10, B[(t+1)%2][x - XSLOPE + STRIDE *2][y]);
			vload(v20, B[t%2    ][x - XSLOPE + STRIDE *1][y]);
			vload(v30, B[(t+1)%2][x - XSLOPE            ][y]);
			transpose(v00, v10, v20, v30, in, out);

		    vload(v01, B[t%2    ][x + STRIDE *3][y]);
		    vload(v11, B[(t+1)%2][x + STRIDE *2][y]);
		    vload(v21, B[t%2    ][x + STRIDE *1][y]);
		    vload(v31, B[(t+1)%2][x            ][y]);
		    transpose(v01, v11, v21, v31, in, out);

			vload(v02, B[t%2    ][x + XSLOPE + STRIDE *3][y]);
			vload(v12, B[(t+1)%2][x + XSLOPE + STRIDE *2][y]);
			vload(v22, B[t%2    ][x + XSLOPE + STRIDE *1][y]);
			vload(v32, B[(t+1)%2][x + XSLOPE            ][y]);
			transpose(v02, v12, v22, v32, in, out);

			vstore( B[(t+1)%2][x - XSLOPE + (STRIDE+1) *3][y], v30);
			vstore( B[(t+1)%2][x - XSLOPE + (STRIDE+1) *2][y], v20);
			vstore( B[(t+1)%2][x - XSLOPE + (STRIDE+1) *1][y], v10);
			vstore( B[(t+1)%2][x - XSLOPE                ][y], v00);

			vstore( B[(t+1)%2][x + (STRIDE+1) *3][y], v31);
			vstore( B[(t+1)%2][x + (STRIDE+1) *2][y], v21);
			vstore( B[(t+1)%2][x + (STRIDE+1) *1][y], v11);
			vstore( B[(t+1)%2][x                ][y], v01);

			vstore( B[(t+1)%2][x + XSLOPE + (STRIDE+1) *3][y], v32);
			vstore( B[(t+1)%2][x + XSLOPE + (STRIDE+1) *2][y], v22);
			vstore( B[(t+1)%2][x + XSLOPE + (STRIDE+1) *1][y], v12);
			vstore( B[(t+1)%2][x + XSLOPE                ][y], v02);
		}

        //------------------------------------------
        // when x = veclen +1 ~ NX + VECLEN - STRIDE * VECLEN
        //------------------------------------------
        for ( xx = XSTART + 1 , x = XSTART +1 ; x <= NX + XSTART - STRIDE * VECLEN ; x ++){
            y = YSTART;
            vloadset(v00, B[(t+1)%2][x-XSLOPE         ][y-YSLOPE], \
                          B[(t)%2  ][x-XSLOPE+STRIDE  ][y-YSLOPE], \
                          B[(t+1)%2][x-XSLOPE+STRIDE*2][y-YSLOPE], \
                          B[(t)%2  ][x-XSLOPE+STRIDE*3][y-YSLOPE]);

            vloadset(v01, B[(t+1)%2][x         ][y-YSLOPE], \
                          B[(t)%2  ][x+STRIDE  ][y-YSLOPE], \
                          B[(t+1)%2][x+STRIDE*2][y-YSLOPE], \
                          B[(t)%2  ][x+STRIDE*3][y-YSLOPE]);

            vloadset(v02, B[(t+1)%2][x+XSLOPE         ][y-YSLOPE], \
                          B[(t)%2  ][x+XSLOPE+STRIDE  ][y-YSLOPE], \
                          B[(t+1)%2][x+XSLOPE+STRIDE*2][y-YSLOPE], \
                          B[(t)%2  ][x+XSLOPE+STRIDE*3][y-YSLOPE]);

            vload(v10, B[(t+1)%2  ][xx + (x-xx-XSLOPE)%3][y]);
            vload(v11, B[(t+1)%2  ][xx + (x-xx)%3       ][y]);
            vload(v12, B[(t+1)%2  ][xx + (x-xx+XSLOPE)%3][y]);

            // printf("In kernel(B) loaded:t=2 %f %f %f %f %f %f %f %f %f \n",\
            //         B[(t+1)%2][x-XSLOPE][y-YSLOPE],     B[(t+1)%2][x][y-YSLOPE],     B[(t+1)%2][x+XSLOPE][y-YSLOPE], \
            //         // B[(t+1)%2  ][x-XSLOPE][y+3], B[(t+1)%2][x][y+3], B[(t+1)%2][x+XSLOPE][y+3]);
            //         B[(t)%2  ][x-XSLOPE+STRIDE*3][y+3], B[(t)%2  ][x+STRIDE*3][y+3], B[(t)%2][x+XSLOPE+STRIDE*3][y+3],\
            //         B[(t+1)%2  ][x-XSLOPE+STRIDE*2][y+3], B[(t+1)%2  ][x+STRIDE*2][y+3], B[(t+1)%2][x+XSLOPE+STRIDE*2][y+3]);

            for ( y = YSTART; y <= NY + YSTART - VECLEN - VECLEN; y+= VECLEN) {
                vload(v20, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)  ][y]);         //(x, y) = (-1, -1) left
                vload(v30, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*2][y]);

                vload(v21, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)  ][y]);                //(x, y) = (0, -1) mid 
                vload(v31, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)*2][y]);

                vload(v22, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)  ][y]);         //(x, y) = (1, -1) right
                vload(v32, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*2][y]);

                vload(in, B[(t)%2][x+STRIDE*4][y]);   // the next x iter in vector

                Compute_1vector(v20, v21, v22,\
                                v10, v11, v12,\
                                v00, v01, v02);   //1st   store the newest value to the left vec
                Input_Output_1(out, v00, in);
                vstore(B[(t+1)%2  ][xx + (x-xx+STRIDE)%3][y], v00);   //for the compute of next x iteration


                Compute_1vector(v30, v31, v32,\
                                v20, v21, v22,\
                                v10, v11, v12);    //2nd
                Input_Output_2(out, v10, in);
                vstore(B[(t+1)%2][xx + (x-xx+STRIDE)%3 + STRIDE+1][y], v10);  //for the compute of next x iteration


                vload(v00, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*3][y]);
                vload(v01, B[(t+1)%2][xx + (x-xx)%3        + (STRIDE+1)*3][y]);
                vload(v02, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*3][y]);
                Compute_1vector(v00, v01, v02,\
                                v30, v31, v32,\
                                v20, v21, v22);   //3rd
                Input_Output_3(out, v20, in);
                vstore(B[(t+1)%2  ][xx + (x-xx+STRIDE)%3 + (STRIDE+1)*2][y], v20);



                vload(v10, B[(t+1)%2][xx + (x-xx-XSLOPE)%3][y+VECLEN]);
                vload(v11, B[(t+1)%2][xx + (x-xx)%3       ][y+VECLEN]);
                vload(v12, B[(t+1)%2][xx + (x-xx+XSLOPE)%3][y+VECLEN]);
                Compute_1vector(v10, v11, v12,\
                                v00, v01, v02,\
                                v30, v31, v32);   //4th
                Input_Output_4(out, v30, in);
                vstore(B[(t)%2][x][y], out);

                vstore(B[(t+1)%2][xx + (x-xx+STRIDE)%3 + (STRIDE+1)*3][y], v30);


            }
            vload(v20, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)  ][y]);         //(x, y) = (-1, -1) left
            vload(v30, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*2][y]);
                
            vload(v21, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)  ][y]);                //(x, y) = (0, -1) mid 
            vload(v31, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)*2][y]);

            vload(v22, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)  ][y]);         //(x, y) = (1, -1) right
            vload(v32, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*2][y]);

            vload(in, B[(t)%2][x+STRIDE*4][y]);   // the next x iter in vector

            Compute_1vector(v20, v21, v22,\
                            v10, v11, v12,\
                            v00, v01, v02);   //1st   store the newest value to the left vec
            Input_Output_1(out, v00, in);
            vstore(B[(t+1)%2  ][xx + (x-xx+STRIDE)%3][y], v00);   //for the compute of next x iteration


            Compute_1vector(v30, v31, v32,\
                            v20, v21, v22,\
                            v10, v11, v12);    //2nd
            Input_Output_2(out, v10, in);
            vstore(B[(t+1)%2][xx + (x-xx+STRIDE)%3 + STRIDE+1][y], v10);  //for the compute of next x iteration


            vload(v00, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*3][y]);
            vload(v01, B[(t+1)%2][xx + (x-xx)%3        + (STRIDE+1)*3][y]);
            vload(v02, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*3][y]);
            Compute_1vector(v00, v01, v02,\
                            v30, v31, v32,\
                            v20, v21, v22);   //3rd
            Input_Output_3(out, v20, in);
            vstore(B[(t+1)%2  ][xx + (x-xx+STRIDE)%3 + (STRIDE+1)*2][y], v20);


            vloadset(v10, B[(t+1)%2][x-XSLOPE         ][y+VECLEN], \
                          B[(t)%2  ][x-XSLOPE+STRIDE  ][y+VECLEN], \
                          B[(t+1)%2][x-XSLOPE+STRIDE*2][y+VECLEN], \
                          B[(t)%2  ][x-XSLOPE+STRIDE*3][y+VECLEN]);

            vloadset(v11, B[(t+1)%2][x         ][y+VECLEN], \
                          B[(t)%2  ][x+STRIDE  ][y+VECLEN], \
                          B[(t+1)%2][x+STRIDE*2][y+VECLEN], \
                          B[(t)%2  ][x+STRIDE*3][y+VECLEN]);

            vloadset(v12, B[(t+1)%2][x+XSLOPE         ][y+VECLEN], \
                          B[(t)%2  ][x+XSLOPE+STRIDE  ][y+VECLEN], \
                          B[(t+1)%2][x+XSLOPE+STRIDE*2][y+VECLEN], \
                          B[(t)%2  ][x+XSLOPE+STRIDE*3][y+VECLEN]);

            Compute_1vector(v10, v11, v12,\
                            v00, v01, v02,\
                            v30, v31, v32);   //4th
            Input_Output_4(out, v30, in);
            vstore(B[(t+1)%2][xx + (x-xx+STRIDE)%3 + (STRIDE+1)*3][y], v30);

            vstore(B[(t)%2][x][y], out);  

            // if(t == 0) printf("In kernel(B) after compute:t=3 %f %f %f %f\n",
            //       B[(t)%2][VECLEN][VECLEN], B[(t)%2][VECLEN][VECLEN+1],\
            //       B[(t)%2][VECLEN][VECLEN+2],B[(t)%2][VECLEN][VECLEN+3]\
            //       );
            
            //extra points along y
            for ( y += VECLEN+1; y <= NY+YSTART; y++){
                vloadset(v20, B[(t+1)%2][x-XSLOPE         ][y], \
                              B[(t)%2  ][x-XSLOPE+STRIDE  ][y], \
                              B[(t+1)%2][x-XSLOPE+STRIDE*2][y], \
                              B[(t)%2  ][x-XSLOPE+STRIDE*3][y]);

                vloadset(v21, B[(t+1)%2][x         ][y], \
                              B[(t)%2  ][x+STRIDE  ][y], \
                              B[(t+1)%2][x+STRIDE*2][y], \
                              B[(t)%2  ][x+STRIDE*3][y]);

                vloadset(v22, B[(t+1)%2][x+XSLOPE         ][y], \
                              B[(t)%2  ][x+XSLOPE+STRIDE  ][y], \
                              B[(t+1)%2][x+XSLOPE+STRIDE*2][y], \
                              B[(t)%2  ][x+XSLOPE+STRIDE*3][y]);

                Compute_1vector(v20, v21, v22,\
                                v10, v11, v12,\
                                v00, v01, v02);
                _mm256_storeu_pd(tmp, v00);

		    	B[(t+1)%2][x + STRIDE * 3][y - 1] = tmp[0];
		    	B[t%2    ][x + STRIDE * 2][y - 1] = tmp[1];
		    	B[(t+1)%2][x + STRIDE * 1][y - 1] = tmp[2];
		    	B[t%2    ][x             ][y - 1] = tmp[3];
                v00 = v10;
                v01 = v11;
                v02 = v12;
                v10 = v20;
                v11 = v21;
                v12 = v22;
                // kernel2(B);		
            }

        }

        //when x  = NX + VECLEN - STRIDE * VECLEN,
        //need extra transpose back to memory

        // for(x = xx - XSLOPE; x <= xx +XSLOPE; x++){
		for ( y = YSTART; y <= NY + YSTART - VECLEN ; y += VECLEN ){
            vload(v00, B[(t+1)%2][xx + (x-xx-XSLOPE)%3][y]);	
            vload(v10, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*1][y]);	
            vload(v20, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*2][y]);
            vload(v30, B[(t+1)%2][xx + (x-xx-XSLOPE)%3 + (STRIDE+1)*3][y]);			
            transpose(v00, v10, v20, v30, in, out);

            vload(v01, B[(t+1)%2][xx + (x-xx)%3       ][y]);
            vload(v11, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)*1][y]);
            vload(v21, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)*2][y]);
            vload(v31, B[(t+1)%2][xx + (x-xx)%3 + (STRIDE+1)*3][y]);
            transpose(v01, v11, v21, v31, in, out);

            vload(v02, B[(t+1)%2][xx + (x-xx+XSLOPE)%3][y]);
            vload(v12, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*1][y]);
            vload(v22, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*2][y]);
            vload(v32, B[(t+1)%2][xx + (x-xx+XSLOPE)%3 + (STRIDE+1)*3][y]);
            transpose(v02, v12, v22, v32, in, out);

			// vstore( B[t%2    ][x -XSLOPE + STRIDE *3][y], v00);
			vstore( B[(t+1)%2][x -XSLOPE + STRIDE *2][y], v10);
			vstore( B[t%2    ][x -XSLOPE + STRIDE *1][y], v20);
			vstore( B[(t+1)%2][x -XSLOPE            ][y], v30);

			// vstore( B[t%2    ][x + STRIDE *3][y], v01);
			vstore( B[(t+1)%2][x + STRIDE *2][y], v11);
			vstore( B[t%2    ][x + STRIDE *1][y], v21);
			vstore( B[(t+1)%2][x            ][y], v31);
            
			// vstore( B[t%2    ][x + XSLOPE + STRIDE *3][y], v02);
			vstore( B[(t+1)%2][x + XSLOPE + STRIDE *2][y], v12);
			vstore( B[t%2    ][x + XSLOPE + STRIDE *1][y], v22);
			vstore( B[(t+1)%2][x + XSLOPE            ][y], v32);
		}
		// } 
        xx = x;
    
        //tail
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
	//Extra points
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
