#include "define.h"

typedef void (*kernel_func)(double *, int, int, int, int, int, int);

static double elapsed_seconds(struct timeval start, struct timeval end) {
	return end.tv_sec - start.tv_sec +
		(end.tv_usec - start.tv_usec) * 1.0e-6;
}

static void run_kernel(const char *name, kernel_func func, double *A,
					   int NX, int NY, int T, int xb, int yb, int tb) {
	struct timeval start, end;

	printf("Running %-42s NX = %d, NY = %d, T = %d, xb = %d, yb = %d, tb = %d\n",
		   name, NX, NY, T, xb, yb, tb);
	fflush(stdout);

	gettimeofday(&start, 0);
	func(A, NX, NY, T, xb, yb, tb);
	gettimeofday(&end, 0);

	double seconds = elapsed_seconds(start, end);
	double gstencil = ((double)NX * NY * T) / seconds / 1000000000.0;
	printf("Finished %-41s time = %.6f s, GStencil/s = %.6f\n",
		   name, seconds, gstencil);
	fflush(stdout);
}

static double read_env_double(const char *name, double default_value) {
	const char *value = getenv(name);

	if (value == NULL || value[0] == '\0') {
		return default_value;
	}
	return atof(value);
}

static int checkresult_big(int NX, int NY,
						   double (*A_correct)[NY + 2 * YSTART],
						   double (*A)[NY + 2 * YSTART],
						   double rtol, double atol) {
	int correct = 1;
	int shown = 0;
	long long strict_mismatch = 0;
	long long tolerance_mismatch = 0;
	double max_abs = 0.0;
	double max_rel = 0.0;
	int max_x = XSTART;
	int max_y = YSTART;

	for (int x = XSTART; x < NX + XSTART; x++) {
		for (int y = YSTART; y < NY + YSTART; y++) {
			double diff = fabs(A[x][y] - A_correct[x][y]);
			double scale = fmax(1.0, fmax(fabs(A[x][y]), fabs(A_correct[x][y])));
			double rel = diff / scale;

			if (diff > max_abs) {
				max_abs = diff;
				max_rel = rel;
				max_x = x;
				max_y = y;
			}
			if (diff > 1e-12 * scale) {
				strict_mismatch++;
			}

			if (diff > atol + rtol * scale) {
				if (shown < 16) {
					printf("x = [%d], y = [%d], Correct = %.15f, Wrong = %.15f\n",
						   x, y, A_correct[x][y], A[x][y]);
				}
				shown++;
				tolerance_mismatch++;
				correct = 0;
			}
		}
	}

	if (shown > 16) {
		printf("More mismatches omitted: %d\n", shown - 16);
	}
	printf("Check summary: max_abs = %.3e, max_rel = %.3e, at x = %d, y = %d\n",
		   max_abs, max_rel, max_x, max_y);
	printf("Check tolerance: atol = %.3e, rtol = %.3e, strict_1e-12_mismatch = %lld, tolerance_mismatch = %lld\n",
		   atol, rtol, strict_mismatch, tolerance_mismatch);
	return correct;
}

int main(int argc, char* argv[]) {
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

	size_t grid_size = (size_t)(NX + 2 * XSTART) * (NY + 2 * YSTART);
	double memory_gb = (double)(grid_size * 4 * sizeof(double)) /
		(1024.0 * 1024.0 * 1024.0);

	printf("Big test memory for two 2-layer arrays: %.3f GiB\n", memory_gb);
	double check_rtol = read_env_double("BIG_CHECK_RTOL", 1e-10);
	double check_atol = read_env_double("BIG_CHECK_ATOL", 1e-12);
	printf("Big test check tolerance: atol = %.3e, rtol = %.3e\n",
		   check_atol, check_rtol);
	fflush(stdout);

	double (*A_scalar)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(sizeof(double) * grid_size * 2);
	double (*A_vectime)[NX + 2 * XSTART][NY + 2 * YSTART] =
		(double (*)[NX + 2 * XSTART][NY + 2 * YSTART])
		malloc(sizeof(double) * grid_size * 2);

	if (A_scalar == NULL || A_vectime == NULL) {
		printf("malloc failed\n");
		free(A_scalar);
		free(A_vectime);
		return 1;
	}

	srand(100);
	for (int x = 0; x < NX + 2 * XSTART; x++) {
		for (int y = 0; y < NY + 2 * YSTART; y++) {
			double value = INIT;
			A_scalar[0][x][y] = value;
			A_scalar[1][x][y] = value;
			A_vectime[0][x][y] = value;
			A_vectime[1][x][y] = value;
		}
	}

	run_kernel("blocking_parallel_rectangle_scalar",
			   blocking_parallel_rectangle_scalar,
			   (double *)A_scalar, NX, NY, T, xb, yb, tb);
	run_kernel("blocking_parallel_rectangle_vectime_extra_array",
			   blocking_parallel_rectangle_vectime_extra_array,
			   (double *)A_vectime, NX, NY, T, xb, yb, tb);

	int correct = checkresult_big(
		NX, NY,
		(double (*)[NY + 2 * YSTART])&A_scalar[T % 2][0][0],
		(double (*)[NY + 2 * YSTART])&A_vectime[T % 2][0][0],
		check_rtol, check_atol);

	printf("%s blocking_parallel_rectangle_vectime_extra_array\n",
		   correct ? "Correct!" : "Wrong!");

	free(A_scalar);
	free(A_vectime);
	return correct ? 0 : 1;
}
