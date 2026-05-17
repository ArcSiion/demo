#include "define.h"

#define init_data(A)	for (x = 0; x < NX + 2 * XSTART; x++) {\
		for (y = 0; y < NY + 2 * YSTART; y++) {\
			A[0][x][y] = A_backup[0][x][y];\
			A[1][x][y] = A_backup[0][x][y];\
		}\
	}

#define run_and_test(func, A)	init_data(A);\
	gettimeofday(&start, 0);\
	func((double *)A, NX, NY, T, xb, yb, tb);\
	gettimeofday(&end, 0);\
	printf("%-10s %-42s NX = %d, NY = %d, T = %d, GStencil/s = %f\n",\
			checkresult(NX, NY,\
				(double (*)[NY + 2 * YSTART])&A_correct[T % 2][0][0],\
				(double (*)[NY + 2 * YSTART])&A[T % 2][0][0]) ? "Correct!" : "Wrong!",\
			#func",", NX, NY, T,\
			((double) NX * NY * T) /\
			(double)(end.tv_sec - start.tv_sec +\
				(end.tv_usec - start.tv_usec) * 1.0e-6) / 1000000000L)

int main(int argc, char* argv[]) {

	struct timeval start, end;

	int x, y;
	if (argc != 7) {
		printf("usage: %s <NX> <NY> <T> <xb> <yb> <tb>\n", argv[0]);
		return 0;
	}

	int NX = atoi(argv[1]);
	int NY = atoi(argv[2]);
	int T  = atoi(argv[3]);
	int xb = atoi(argv[4]);
	int yb = atoi(argv[5]);
	int tb = atoi(argv[6]);

	if (tb > (xb - 1) / (2 * XSLOPE)) {
		tb = (xb - 1) / (2 * XSLOPE);
	}
	tb = max(tb, 1);

	double (*A)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(sizeof(double) * (NX + 2 * XSTART) * (NY + 2 * YSTART) * 2);
	double (*A_correct)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(sizeof(double) * (NX + 2 * XSTART) * (NY + 2 * YSTART) * 2);
	double (*A_backup)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(sizeof(double) * (NX + 2 * XSTART) * (NY + 2 * YSTART));

	srand(100);
	for (x = 0; x < NX + 2 * XSTART; x++) {
		for (y = 0; y < NY + 2 * YSTART; y++) {
			A_backup[0][x][y] = INIT;
		}
	}

	run_and_test(naive_vector, A_correct);
	run_and_test(blocking_parallel_rectangle_scalar, A);
	run_and_test(blocking_parallel_rectangle_vector, A);
	run_and_test(blocking_parallel_rectangle_vectime_extra_array, A);

	free(A);
	free(A_correct);
	free(A_backup);
	return 0;
}

int checkresult(int NX, int NY, double (* A_correct)[NY + 2 * YSTART],
				double (* A)[NY + 2 * YSTART]) {
	int correct = 1;
	int x, y;

	for (x = XSTART; x < NX + XSTART; x++) {
		for (y = YSTART; y < NY + YSTART; y++) {
			if (fabs(A[x][y] - A_correct[x][y]) >
				1e-12 * fmax(1.0, fmax(fabs(A[x][y]), fabs(A_correct[x][y])))) {
				printf("x = [%d], y = [%d], Correct = %.15f, Wrong = %.15f\n",
					   x, y, A_correct[x][y], A[x][y]);
				correct = 0;
			}
		}
	}

	return correct;
}
