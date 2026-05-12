

EXE=exe-3d-jacobi-27p-blocking

include ../common.make

all: $(EXE)

$(EXE):  defines.h	main.c		blocking_parallel_rectangle_vector.c blocking_parallel_rectangle_scalar.c	naive_vector.c blocking_parallel_rectangle_vectime_extra_array.c blocking_parallel_rectangle_vectime_extra_array_512.c
	$(CC) $(OPT_FLAGS) $(OMP_FLAGS) $^ -o $(EXE) -lm

clean:
	rm -f  exe* 


