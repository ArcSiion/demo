#include "define.h"

static double big_check_rtol(void)
{
	char *value = getenv("BIG_CHECK_RTOL");

	return value == NULL ? 1e-10 : atof(value);
}

static double big_check_atol(void)
{
	char *value = getenv("BIG_CHECK_ATOL");

	return value == NULL ? 1e-12 : atof(value);
}

#define init_data(A)	for (x = 0; x < NX + 2 * XSTART; x++) {\
		for (y = 0; y < NY + 2 * YSTART; y++) {\
			A[0][x][y] = A_backup[0][x][y];\
			A[1][x][y] = A_backup[0][x][y];\
		}\
	}

#define run_func(func, A)	init_data(A);\
	printf("Running %-42s NX = %d, NY = %d, T = %d, xb = %d, yb = %d, tb = %d\n",\
		   #func, NX, NY, T, xb, yb, tb);\
	gettimeofday(&start, 0);\
	func((double *)A, NX, NY, T, xb, yb, tb);\
	gettimeofday(&end, 0);\
	printf("Finished %-41s time = %.6f s, GStencil/s = %f\n",\
		   #func,\
		   (double)(end.tv_sec - start.tv_sec +\
			   (end.tv_usec - start.tv_usec) * 1.0e-6),\
		   ((double) NX * NY * T) /\
		   (double)(end.tv_sec - start.tv_sec +\
			   (end.tv_usec - start.tv_usec) * 1.0e-6) / 1000000000L)

#define run_and_check(func, A)	run_func(func, A);\
	printf("%s %s\n",\
		   checkresult(NX, NY,\
			   (double (*)[NY + 2 * YSTART])&A_correct[T % 2][0][0],\
			   (double (*)[NY + 2 * YSTART])&A[T % 2][0][0]) ?\
			   "Correct!" : "Wrong!",\
		   #func)

int main(int argc, char* argv[])
{
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

	size_t layer_size = sizeof(double) *
		(NX + 2 * XSTART) * (NY + 2 * YSTART);
	printf("Big test memory for two 2-layer arrays: %.3f GiB\n",
		   (double)(layer_size * 4) / (1024.0 * 1024.0 * 1024.0));

	double (*A)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(layer_size * 2);
	double (*A_correct)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(layer_size * 2);
	double (*A_backup)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(layer_size);

	srand(100);
	for (x = 0; x < NX + 2 * XSTART; x++) {
		for (y = 0; y < NY + 2 * YSTART; y++) {
			A_backup[0][x][y] = INIT;
		}
	}

	run_func(blocking_parallel_rectangle_scalar, A_correct);
	run_and_check(blocking_parallel_rectangle_vector, A);
	run_and_check(blocking_parallel_rectangle_vectime_extra_array, A);

	free(A);
	free(A_correct);
	free(A_backup);
	return 0;
}

int checkresult(int NX, int NY, double (* A_correct)[NY + 2 * YSTART],
				double (* A)[NY + 2 * YSTART])
{
	int correct = 1;
	int x, y;
	double rtol = big_check_rtol();
	double atol = big_check_atol();
	double max_abs_err = 0.0;
	double max_rel_err = 0.0;
	int max_x = XSTART;
	int max_y = YSTART;

	for (x = XSTART; x < NX + XSTART; x++) {
		for (y = YSTART; y < NY + YSTART; y++) {
			double abs_err = fabs(A[x][y] - A_correct[x][y]);
			double scale = fmax(1.0, fmax(fabs(A[x][y]), fabs(A_correct[x][y])));
			double rel_err = abs_err / scale;

			if (abs_err > max_abs_err) {
				max_abs_err = abs_err;
				max_rel_err = rel_err;
				max_x = x;
				max_y = y;
			}
			if (abs_err > atol && rel_err > rtol) {
				printf("x = [%d], y = [%d], Correct = %.15f, Wrong = %.15f\n",
					   x, y, A_correct[x][y], A[x][y]);
				correct = 0;
			}
		}
	}
	printf("max_abs_err = %.6e, max_rel_err = %.6e at x = %d, y = %d\n",
		   max_abs_err, max_rel_err, max_x, max_y);
	return correct;
}
