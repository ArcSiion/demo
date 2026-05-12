#include "defines.h"

void blocking_parallel_rectangle_vectime_extra_array_512(double *A, int NX, int NY, int NZ, int T, int xb, int yb, int zb, int tb) {

    double (* B)[NX + 2 * XSTART][ NY + 2 * YSTART][ NZ + 2 * ZSTART] =  (double (*)[NX + 2 * XSTART][ NY + 2 * YSTART][ NZ + 2 * ZSTART]) A;
#ifdef scalar_ratio
    long long cnt = 0;
#endif
    tb -= tb % VVECLEN;
    if (tb < VVECLEN) tb = VVECLEN;
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
    int tt, xx, yy, zz, t, tv, x, y, z;
    int xmin, xmax, ybeg, zbeg, myybeg, myzbeg;
    int myyb, myzb;

    const int xblocknum = max(nb0[0], nb0[1]);
    const int yblocknum = 2 * myceil(NY + T - 1, yb);
    const int zblocknum = 2 * myceil(NZ + T - 1, zb);

    int myid = omp_get_max_threads();

    double (* AV) [(yb + 2)][(zb + 2)][VVECLEN] = (double(*)[(yb + 2)][(zb + 2)][VVECLEN])alloc_extra_array_512(sizeof(double) * (yb + 2) * (zb + 2) * VVECLEN * 4 * myid);

    for (wave = 0; wave < myceil(T, tb) + 1 + myceil(NY + T - 1, yb) + myceil(NZ + T - 1, zb); wave++) {

        #pragma omp parallel for private(myid, mylevel, t, tt, tv, x, y, z, xx, yy, zz, xmin, xmax, ybeg, zbeg, myybeg, myzbeg, myyb, myzb) collapse(3) schedule(dynamic, 1)

        for (xx = 0; xx < xblocknum; xx++){
            for (yy = 0; yy < yblocknum; yy++){
                for (zz = 0; zz < zblocknum; zz++){

                    double tmp[VVECLEN];

                    __m512d v_center_0, v_center_1, v_center_2, v_center_3;
                    __m512d v_center_4, v_center_5, v_center_6, v_center_7;
                    __m512d v_all_d_1_0, v_all_d_1_1, v_all_d_1_2;
                    __m512d v_all_d_2_0, v_all_d_2_1, v_all_d_2_2;
                    __m512d in, out;
                    SET_COFF_512;

                    myid = omp_get_thread_num();
                    double (* BV0) [zb][VVECLEN] = (double(*)[zb][VVECLEN]) (AV + myid * 4 + 0);
                    double (* BV1) [zb][VVECLEN] = (double(*)[zb][VVECLEN]) (AV + myid * 4 + 1);
                    double (* BV2) [zb][VVECLEN] = (double(*)[zb][VVECLEN]) (AV + myid * 4 + 2);
                    double (* BV3) [zb][VVECLEN] = (double(*)[zb][VVECLEN]) (AV + myid * 4 + 3);
                    double (* Btmp [4]) [zb][VVECLEN]  = {BV0, BV1, BV2, BV3};

                    mylevel = (wave % 2 + yy + zz) % 2;

                    if(xx < nb0[mylevel]){

                        tt = -tb + (wave - yy - zz) * tb;

                        ybeg = YSTART + yy * yb - (wave - yy - zz) * tb;
                        zbeg = ZSTART + zz * zb - (wave - yy - zz) * tb;

                        for (tv = max(tt, 0); tv <= min(tt + 2 * tb, T) - VVECLEN; tv += VVECLEN, ybeg -= VVECLEN, zbeg -= VVECLEN) {

                            if(ybeg - VVECLEN + 1 < YSTART){
                                myybeg  =   YSTART + VVECLEN - 1;
                                myyb    =   yb - (myybeg - ybeg);
                            } else {
                                myybeg  =   ybeg;
                                myyb    =   yb;
                            }
                            if (ybeg + yb >= YSTART + NY){
                                myyb -= ybeg + yb - (YSTART + NY);
                            }

                            if(zbeg - VVECLEN + 1 < ZSTART){
                                myzbeg  =   ZSTART + VVECLEN - 1;
                                myzb    =   zb - (myzbeg - zbeg);
                            } else {
                                myzbeg  =   zbeg;
                                myzb    =   zb;
                            }
                            if (zbeg + zb >= ZSTART + NZ){
                                myzb -= zbeg + zb - (ZSTART + NZ);
                            }

                            if (    xb   <= myxb_threshold_512 \
                                ||  myyb <= myyb_threshold_512 \
                                ||  myzb <= myzb_threshold_512 )
                            {
                                for (t = tv; t < tv + VVECLEN; t++) {
                                    xmin = (mylevel == 1 && xx == 0) ?               XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
                                    xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);

                                    for (x = xmin; x < xmax; x++) {
                                        for (y = max(YSTART, ybeg - (t - tv)); y < min(NY + YSTART, ybeg - (t - tv) + yb); y++) {
                                            #pragma ivdep
                                            #pragma vector always
                                            for (z = max(ZSTART, zbeg - (t - tv)); z < min(NZ + ZSTART, zbeg - (t - tv) + zb); z++) {
                                                Compute_scalar(B, t, x, y, z);
                                            }
                                        }
                                    }
                                }
                            }
                            else {
                                //head
                                for (t = tv; t < tv + VVECLEN - 1; t++){

                                    xmin = (mylevel == 1 && xx == 0) ?               XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
                                    xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);

                                    for (x = xmin; x < xmax; x++) {
                                        for ( y = max(YSTART, ybeg - (t - tv)); y < myybeg - (t - tv); y++){
                                            #pragma vector always
                                            #pragma ivdep
                                            for (z = max(ZSTART, zbeg - (t - tv)); z < myzbeg - (t - tv) + myzb; z++)
                                            {
                                                Compute_scalar(B, t, x, y, z);
                                            }
                                        }
                                    }
                                    for (x = xmin; x < xmax; x++) {
                                        for ( y = myybeg - (t - tv); y < min(NY + YSTART, ybeg + yb - (t - tv)); y++){
                                            #pragma vector always
                                            #pragma ivdep
                                            for (z = max(ZSTART, zbeg - (t - tv)); z < myzbeg - (t - tv); z++)
                                            {
                                                Compute_scalar(B, t, x, y, z);
                                            }
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
                                        for (y = myybeg - (t - tv); y < myybeg - (t - tv) + myyb ; y++) {
                                            #pragma vector always
                                            #pragma ivdep
                                            for (z = myzbeg - (t - tv); z < myzbeg - (t - tv) + myzb ; z++) {
                                                Compute_scalar(B, t, x, y, z);
                                            }
                                        }
                                    }
                                }

                                t = tv;

                                xmin += 1;
                                xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel] + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);

                                // ====== HEAD transpose: B → Btmp ======
                                for (x = xmin; x < xmin + STRIDE + 1; x++) {

                                    for (y = myybeg - VVECLEN + 1; y < myybeg - VVECLEN + 1 + myyb; y++) {

                                        for (z = myzbeg - VVECLEN + 1; z <= myzbeg - VVECLEN + 1 + myzb - VVECLEN; z += VVECLEN) {

                                            vload_512_blk(v_center_0, B[t%2    ][x + STRIDE *7][y+7][z+7]);
                                            vload_512_blk(v_center_1, B[(t+1)%2][x + STRIDE *6][y+6][z+6]);
                                            vload_512_blk(v_center_2, B[t%2    ][x + STRIDE *5][y+5][z+5]);
                                            vload_512_blk(v_center_3, B[(t+1)%2][x + STRIDE *4][y+4][z+4]);
                                            vload_512_blk(v_center_4, B[t%2    ][x + STRIDE *3][y+3][z+3]);
                                            vload_512_blk(v_center_5, B[(t+1)%2][x + STRIDE *2][y+2][z+2]);
                                            vload_512_blk(v_center_6, B[t%2    ][x + STRIDE *1][y+1][z+1]);
                                            vload_512_blk(v_center_7, B[(t+1)%2][x            ][y  ][z  ]);

                                            transpose_512(v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7, in, out);

                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 0][0], v_center_0);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 1][0], v_center_1);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 2][0], v_center_2);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 3][0], v_center_3);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 4][0], v_center_4);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 5][0], v_center_5);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 6][0], v_center_6);
                                            _mm512_storeu_pd(&Btmp[x - xmin][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 7][0], v_center_7);

                                        }
                                    }
                                }

                                // ====== MAIN 8-step body ======
                                for (x = xmin + XSLOPE; x < xmax - STRIDE * VVECLEN; x++) {

                                    for (y = myybeg - VVECLEN + 1; y < myybeg - VVECLEN + 1 + myyb; y++) {

                                        z = myzbeg - VVECLEN + 1;

                                        v_center_0 = load_v_512(x, y, z, 0, 0, -1);

                                        if(y < myybeg - VVECLEN + 1 + myyb - 1)
                                            v_center_7 = load_v_512(x + 2, y, z, 0, 1, 0);

                                        v_center_1 = load_x_c_blocking_512(x, y, z, 0, 0);

                                        v_all_d_1_0 = Add_4_vectors_512(    load_v_512(x, y, z, -1, 0, -1),\
                                                                            load_v_512(x, y, z, 1, 0, -1),\
                                                                            load_v_512(x, y, z, 0, 1, -1),\
                                                                            load_v_512(x, y, z, 0, -1, -1));
                                        v_all_d_2_0 = Add_4_vectors_512(    load_v_512(x, y, z, -1, 1, -1),\
                                                                            load_v_512(x, y, z, -1, -1, -1),\
                                                                            load_v_512(x, y, z, 1, 1, -1),\
                                                                            load_v_512(x, y, z, 1, -1, -1));

                                        v_all_d_1_1 = Add_4_d_1_512(x, y, z, 0);
                                        v_all_d_2_1 = Add_4_d_2_512(x, y, z, 0);

                                        for (; z <= myzbeg - VVECLEN + 1 + myzb - VVECLEN; z += VVECLEN) {

                                            vload_512_blk(in, B[(t)%2][x+STRIDE*8][y+VVECLEN][z+VVECLEN]);
                                            if(y < myybeg - VVECLEN + 1 + myyb-1) {
                                                store_x_pp_512(v_center_7, x, y, z, 1, 0);
                                            }

                                            // ---- Step 1: target=v_center_0 ----
                                            v_center_2 = load_x_c_blocking_512(x, y, z, 0, 1);
                                            v_all_d_1_2 = Add_4_d_1_512(x, y, z, 1);
                                            v_all_d_2_2 = Add_4_d_2_512(x, y, z, 1);
                                            Compute_1vector_512(v_center_0, v_center_1, v_center_2, v_center_7,\
                                                                v_all_d_1_0, v_all_d_1_1, v_all_d_1_2,\
                                                                v_all_d_2_0, v_all_d_2_1, v_all_d_2_2);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_1_512(out, v_center_0, in);
                                                store_x_pp_512(v_center_0, x, y, z, 1, 1);
                                            } else {
                                                store_v_512(v_center_0, x, y, z, 0, 0, 0);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 2: target=v_center_1 ----
                                            v_center_3 = load_x_c_blocking_512(x, y, z, 0, 2);
                                            v_all_d_1_0 = Add_4_d_1_512(x, y, z, 2);
                                            v_all_d_2_0 = Add_4_d_2_512(x, y, z, 2);
                                            Compute_1vector_512(v_center_1, v_center_2, v_center_3, v_center_0,\
                                                                v_all_d_1_1, v_all_d_1_2, v_all_d_1_0,\
                                                                v_all_d_2_1, v_all_d_2_2, v_all_d_2_0);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_2_512(out, v_center_1, in);
                                                store_x_pp_512(v_center_1, x, y, z, 1, 2);
                                            } else {
                                                store_v_512(v_center_1, x, y, z, 0, 0, 1);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 3: target=v_center_2 ----
                                            v_center_4 = load_x_c_blocking_512(x, y, z, 0, 3);
                                            v_all_d_1_1 = Add_4_d_1_512(x, y, z, 3);
                                            v_all_d_2_1 = Add_4_d_2_512(x, y, z, 3);
                                            Compute_1vector_512(v_center_2, v_center_3, v_center_4, v_center_1,\
                                                                v_all_d_1_2, v_all_d_1_0, v_all_d_1_1,\
                                                                v_all_d_2_2, v_all_d_2_0, v_all_d_2_1);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_3_512(out, v_center_2, in);
                                                store_x_pp_512(v_center_2, x, y, z, 1, 3);
                                            } else {
                                                store_v_512(v_center_2, x, y, z, 0, 0, 2);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 4: target=v_center_3 ----
                                            v_center_5 = load_x_c_blocking_512(x, y, z, 0, 4);
                                            v_all_d_1_2 = Add_4_d_1_512(x, y, z, 4);
                                            v_all_d_2_2 = Add_4_d_2_512(x, y, z, 4);
                                            Compute_1vector_512(v_center_3, v_center_4, v_center_5, v_center_2,\
                                                                v_all_d_1_0, v_all_d_1_1, v_all_d_1_2,\
                                                                v_all_d_2_0, v_all_d_2_1, v_all_d_2_2);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_4_512(out, v_center_3, in);
                                                store_x_pp_512(v_center_3, x, y, z, 1, 4);
                                            } else {
                                                store_v_512(v_center_3, x, y, z, 0, 0, 3);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 5: target=v_center_4 ----
                                            v_center_6 = load_x_c_blocking_512(x, y, z, 0, 5);
                                            v_all_d_1_0 = Add_4_d_1_512(x, y, z, 5);
                                            v_all_d_2_0 = Add_4_d_2_512(x, y, z, 5);
                                            Compute_1vector_512(v_center_4, v_center_5, v_center_6, v_center_3,\
                                                                v_all_d_1_1, v_all_d_1_2, v_all_d_1_0,\
                                                                v_all_d_2_1, v_all_d_2_2, v_all_d_2_0);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_5_512(out, v_center_4, in);
                                                store_x_pp_512(v_center_4, x, y, z, 1, 5);
                                            } else {
                                                store_v_512(v_center_4, x, y, z, 0, 0, 4);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 6: target=v_center_5 (loads v_center_7 overwriting carry) ----
                                            v_center_7 = load_x_c_blocking_512(x, y, z, 0, 6);
                                            v_all_d_1_1 = Add_4_d_1_512(x, y, z, 6);
                                            v_all_d_2_1 = Add_4_d_2_512(x, y, z, 6);
                                            Compute_1vector_512(v_center_5, v_center_6, v_center_7, v_center_4,\
                                                                v_all_d_1_2, v_all_d_1_0, v_all_d_1_1,\
                                                                v_all_d_2_2, v_all_d_2_0, v_all_d_2_1);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_6_512(out, v_center_5, in);
                                                store_x_pp_512(v_center_5, x, y, z, 1, 6);
                                            } else {
                                                store_v_512(v_center_5, x, y, z, 0, 0, 5);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 7: target=v_center_6 ----
                                            v_center_0 = load_x_c_blocking_512(x, y, z, 0, 7);
                                            v_all_d_1_2 = Add_4_d_1_512(x, y, z, 7);
                                            v_all_d_2_2 = Add_4_d_2_512(x, y, z, 7);
                                            Compute_1vector_512(v_center_6, v_center_7, v_center_0, v_center_5,\
                                                                v_all_d_1_0, v_all_d_1_1, v_all_d_1_2,\
                                                                v_all_d_2_0, v_all_d_2_1, v_all_d_2_2);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                Input_Output_7_512(out, v_center_6, in);
                                                store_x_pp_512(v_center_6, x, y, z, 1, 7);
                                            } else {
                                                store_v_512(v_center_6, x, y, z, 0, 0, 6);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- Step 8: target=v_center_7 (boundary check for z+VVECLEN) ----
                                            if ( z > myzbeg - VVECLEN + 1 + myzb - VVECLEN - VVECLEN ){
                                                v_center_1  = load_v_512(x, y, z, 0, 0, VVECLEN);
                                                v_all_d_1_0 = Add_4_vectors_512(    load_v_512(x, y, z, -1, 0, VVECLEN),\
                                                                                    load_v_512(x, y, z, 1, 0, VVECLEN),\
                                                                                    load_v_512(x, y, z, 0, 1, VVECLEN),\
                                                                                    load_v_512(x, y, z, 0, -1, VVECLEN));
                                                v_all_d_2_0 = Add_4_vectors_512(    load_v_512(x, y, z, -1, 1, VVECLEN),\
                                                                                    load_v_512(x, y, z, -1, -1, VVECLEN),\
                                                                                    load_v_512(x, y, z, 1, 1, VVECLEN),\
                                                                                    load_v_512(x, y, z, 1, -1, VVECLEN));
                                            } else {
                                                v_center_1  = load_x_c_blocking_512(x, y, z, 0, VVECLEN);
                                                v_all_d_1_0 = Add_4_d_1_512(x, y, z, VVECLEN);
                                                v_all_d_2_0 = Add_4_d_2_512(x, y, z, VVECLEN);
                                            }

                                            Compute_1vector_512(v_center_7, v_center_0, v_center_1, v_center_6,\
                                                                v_all_d_1_1, v_all_d_1_2, v_all_d_1_0,\
                                                                v_all_d_2_1, v_all_d_2_2, v_all_d_2_0);
                                            if(y < myybeg - VVECLEN + 1 + myyb - 1) {
                                                if (z > myzbeg - VVECLEN + 1 + myzb - VVECLEN - VVECLEN) {
                                                    store_v_512(v_center_7, x, y, z, 0, 0, 7);
                                                }
                                                Input_Output_8_512(out, v_center_7, in);
                                                vstore_512_blk(B[(t)%2][x][y][z], out);
                                            } else {
                                                store_v_512(v_center_7, x, y, z, 0, 0, 7);
                                            }
                                            //-------------------------------------------------------------------------------------------

                                            // ---- end-of-iter LRU rotation for d_1 / d_2 (mod 3 misaligned with mod 8) ----
                                            // before:  d_1_0 = z+8 (newest),  d_1_1 = z+6,  d_1_2 = z+7
                                            // need:    d_1_0 = z+7 (= z-1 of next iter),  d_1_1 = z+8 (= z+0 of next iter)
                                            {
                                                __m512d _tmp_d1 = v_all_d_1_0;
                                                __m512d _tmp_d2 = v_all_d_2_0;
                                                v_all_d_1_0 = v_all_d_1_2;
                                                v_all_d_2_0 = v_all_d_2_2;
                                                v_all_d_1_1 = _tmp_d1;
                                                v_all_d_2_1 = _tmp_d2;
                                            }

                                        }
                                        // ---- Boundary writeback from BV0 (y interior, z exhausted) ----
                                        if(z >= myzbeg - VVECLEN + 1 + myzb && y < myybeg - VVECLEN + 1 + myyb - 1){
                                            B[(t ) % 2   ][x-1 + STRIDE * 7][y+7][z+6] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][0];
                                            B[(t + 1) % 2][x-1 + STRIDE * 6][y+6][z+5] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][1];
                                            B[(t) % 2    ][x-1 + STRIDE * 5][y+5][z+4] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][2];
                                            B[(t + 1) % 2][x-1 + STRIDE * 4][y+4][z+3] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][3];
                                            B[(t) % 2    ][x-1 + STRIDE * 3][y+3][z+2] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][4];
                                            B[(t + 1) % 2][x-1 + STRIDE * 2][y+2][z+1] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][5];
                                            B[(t) % 2    ][x-1 + STRIDE * 1][y+1][z  ] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][6];
                                            B[(t + 1) % 2][x-1             ][y  ][z-1] = BV0[y - (myybeg - VVECLEN + 1)][-1 + z - (myzbeg - VVECLEN + 1)][7];
                                        }
                                        // ---- z scalar tail (still vector but per-z step compute) ----
                                        for ( ; z < myzbeg - VVECLEN + 1 + myzb; z++) {
                                            v_all_d_1_2 = Add_4_vectors_512(    load_v_512(x, y, z, -1, 0, 1),\
                                                                                load_v_512(x, y, z, 1, 0, 1),\
                                                                                load_v_512(x, y, z, 0, 1, 1),\
                                                                                load_v_512(x, y, z, 0, -1, 1));
                                            v_all_d_2_2 = Add_4_vectors_512(    load_v_512(x, y, z, -1, 1, 1),\
                                                                                load_v_512(x, y, z, -1, -1, 1),\
                                                                                load_v_512(x, y, z, 1, 1, 1),\
                                                                                load_v_512(x, y, z, 1, -1, 1));
                                            v_center_2 = load_v_512(x, y, z, 0, 0, 1);
                                            Compute_1vector_512(v_center_0, v_center_1, v_center_2, v_center_7,\
                                                                v_all_d_1_0, v_all_d_1_1, v_all_d_1_2,\
                                                                v_all_d_2_0, v_all_d_2_1, v_all_d_2_2);
                                            store_v_512(v_center_0, x, y, z, 0, 0, 0);
                                            v_center_0 = v_center_1;
                                            v_center_1 = v_center_2;
                                            v_all_d_1_0 = v_all_d_1_1;
                                            v_all_d_2_0 = v_all_d_2_1;
                                            v_all_d_1_1 = v_all_d_1_2;
                                            v_all_d_2_1 = v_all_d_2_2;
                                        }
                                    }
                                    // ---- y = first row (myybeg - VVECLEN + 1): tail head transpose B → BV3 ----
                                    y = myybeg - VVECLEN + 1;
                                    for (z = myzbeg - VVECLEN + 1; z <= myzbeg - VVECLEN + 1 + myzb - VVECLEN; z += VVECLEN) {

                                        vload_512_blk(v_center_0, B[t%2    ][x + XSLOPE + 1 + STRIDE *7][y+7][z+7]);
                                        vload_512_blk(v_center_1, B[(t+1)%2][x + XSLOPE + 1 + STRIDE *6][y+6][z+6]);
                                        vload_512_blk(v_center_2, B[t%2    ][x + XSLOPE + 1 + STRIDE *5][y+5][z+5]);
                                        vload_512_blk(v_center_3, B[(t+1)%2][x + XSLOPE + 1 + STRIDE *4][y+4][z+4]);
                                        vload_512_blk(v_center_4, B[t%2    ][x + XSLOPE + 1 + STRIDE *3][y+3][z+3]);
                                        vload_512_blk(v_center_5, B[(t+1)%2][x + XSLOPE + 1 + STRIDE *2][y+2][z+2]);
                                        vload_512_blk(v_center_6, B[t%2    ][x + XSLOPE + 1 + STRIDE *1][y+1][z+1]);
                                        vload_512_blk(v_center_7, B[(t+1)%2][x + XSLOPE + 1            ][y  ][z  ]);

                                        transpose_512(v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7, in, out);

                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 7][0], v_center_7);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 6][0], v_center_6);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 5][0], v_center_5);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 4][0], v_center_4);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 3][0], v_center_3);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 2][0], v_center_2);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 1][0], v_center_1);
                                        vstore_512_blk( BV3[0][z - (myzbeg - VVECLEN + 1) + 0][0], v_center_0);

                                    }
                                    // ---- y = last row (myybeg - VVECLEN + 1 + myyb - 1): tail transpose BV0 → B ----
                                    y = myybeg - VVECLEN + 1 + myyb - 1;
                                    for (z = myzbeg - VVECLEN + 1; z <= myzbeg - VVECLEN + 1 + myzb - VVECLEN; z += VVECLEN) {

                                        vload_512_blk(v_center_7, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 7][0]);
                                        vload_512_blk(v_center_6, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 6][0]);
                                        vload_512_blk(v_center_5, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 5][0]);
                                        vload_512_blk(v_center_4, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 4][0]);
                                        vload_512_blk(v_center_3, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 3][0]);
                                        vload_512_blk(v_center_2, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 2][0]);
                                        vload_512_blk(v_center_1, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 1][0]);
                                        vload_512_blk(v_center_0, BV0[myyb - 1][z - (myzbeg - VVECLEN + 1) + 0][0]);

                                        transpose_512(v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7, in, out);

                                        vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *7][y+7][z+7], v_center_0);
                                        vstore_512_blk( B[(t+1)%2][x - XSLOPE + STRIDE *6][y+6][z+6], v_center_1);
                                        vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *5][y+5][z+5], v_center_2);
                                        vstore_512_blk( B[(t+1)%2][x - XSLOPE + STRIDE *4][y+4][z+4], v_center_3);
                                        vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *3][y+3][z+3], v_center_4);
                                        vstore_512_blk( B[(t+1)%2][x - XSLOPE + STRIDE *2][y+2][z+2], v_center_5);
                                        vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *1][y+1][z+1], v_center_6);
                                        vstore_512_blk( B[(t+1)%2][x - XSLOPE            ][y  ][z  ], v_center_7);
                                    }
                                    Btmp[0] = BV0;
                                    BV0 = BV1;
                                    BV1 = BV2;
                                    BV2 = BV3;
                                    BV3 = Btmp[0];
                                }
                                Btmp [0] = BV0;
                                Btmp [1] = BV1;
                                Btmp [2] = BV2;

                                // ---- Tail transpose Btmp → B for x = xmax-STRIDE*VVECLEN .. xmax-STRIDE*VVECLEN+2 ----
                                for (; x < xmax - STRIDE * VVECLEN + 3; x++) {

                                    for ( y = myybeg - VVECLEN + 1; y < myybeg - VVECLEN + 1 + myyb; y++) {

                                        for (z = myzbeg - VVECLEN + 1; z <= myzbeg - VVECLEN + 1 + myzb - VVECLEN; z += VVECLEN) {
                                            vload_512_blk(v_center_7, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 7][0]);
                                            vload_512_blk(v_center_6, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 6][0]);
                                            vload_512_blk(v_center_5, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 5][0]);
                                            vload_512_blk(v_center_4, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 4][0]);
                                            vload_512_blk(v_center_3, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 3][0]);
                                            vload_512_blk(v_center_2, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 2][0]);
                                            vload_512_blk(v_center_1, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 1][0]);
                                            vload_512_blk(v_center_0, Btmp[x - (xmax - STRIDE * VVECLEN)][y - (myybeg - VVECLEN + 1)][z - (myzbeg - VVECLEN + 1) + 0][0]);

                                            transpose_512(v_center_0, v_center_1, v_center_2, v_center_3, v_center_4, v_center_5, v_center_6, v_center_7, in, out);

                                            vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *7][y+7][z+7], v_center_0);
                                            vstore_512_blk( B[(t+1)%2][x - XSLOPE + STRIDE *6][y+6][z+6], v_center_1);
                                            vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *5][y+5][z+5], v_center_2);
                                            vstore_512_blk( B[(t+1)%2][x - XSLOPE + STRIDE *4][y+4][z+4], v_center_3);
                                            vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *3][y+3][z+3], v_center_4);
                                            vstore_512_blk( B[(t+1)%2][x - XSLOPE + STRIDE *2][y+2][z+2], v_center_5);
                                            vstore_512_blk( B[t%2    ][x - XSLOPE + STRIDE *1][y+1][z+1], v_center_6);
                                            vstore_512_blk( B[(t+1)%2][x - XSLOPE            ][y  ][z  ], v_center_7);
                                        }
                                    }
                                }

                                xmin = xmax - STRIDE * VVECLEN;
                                for (t = tv; t < tv + VVECLEN; t++) {

                                    xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel] + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);

                                    for (x = xmin + STRIDE * (VVECLEN - 1 - (t - tv)); x < xmax; x++) {

                                        for (y = myybeg - (t - tv); y < myybeg - (t - tv) + myyb; y++) {
                                            #pragma ivdep
                                            #pragma vector always
                                            for (z = myzbeg - (t - tv); z < myzbeg - (t - tv) + myzb ; z++) {
                                                Compute_scalar(B, t, x, y, z);
                                            }
                                        }
                                    }
                                }

                                for (t = tv; t < tv + VVECLEN; t++){

                                    xmin = (mylevel == 1 && xx == 0) ?               XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
                                    xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);

                                    for (x = xmin; x < xmax; x++) {

                                        for ( y = myybeg + myyb - (t - tv); y < min(NY + YSTART, ybeg + yb - (t - tv)); y++) {
                                            #pragma vector always
                                            #pragma ivdep
                                            for (z = myzbeg - (t - tv); z < myzbeg - (t - tv) + myzb; z++)
                                            {
                                                Compute_scalar(B, t, x, y, z);
                                            }
                                        }
                                    }
                                    for (x = xmin; x < xmax; x++) {

                                        for ( y = max(YSTART, ybeg - (t - tv)); y < min(NY + YSTART, ybeg + yb - (t - tv)); y++){
                                            #pragma vector always
                                            #pragma ivdep
                                            for (z = myzbeg + myzb - (t - tv); z < min(NZ + ZSTART, zbeg + zb - (t - tv)); z++)
                                            {
                                                Compute_scalar(B, t, x, y, z);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //extra
                        //
                        for (t = tv; t < min(tt + 2 * tb, T); t++) {

                            xmin = (mylevel == 1 && xx == 0) ?               XSTART : (xright[mylevel] - Bx + xx * ix + myabs((tt + tb), (t + 1)) * XSLOPE);
                            xmax = (mylevel == 1 && xx == nb0[1] - 1) ? NX + XSTART : (xright[mylevel]      + xx * ix - myabs((tt + tb), (t + 1)) * XSLOPE);

                            for (x = xmin; x < xmax; x++) {

                                for (y = max(YSTART, ybeg - (t - tv)); y < min(NY + YSTART, ybeg - (t - tv) + yb); y++) {
                                    #pragma ivdep
                                    #pragma vector always
                                    for (z = max(ZSTART, zbeg - (t - tv)); z < min(NZ + ZSTART, zbeg - (t - tv) + zb); z++){
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
    free_extra_array(AV);
 #ifdef scalar_ratio
    printf("%f\n", (double) cnt /(double)((double)NX * (double) NY *(double) NZ * (double) T));
#endif
}
