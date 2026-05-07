

EXE=exe-2d-jacobi-9p

include ../common.make

all: $(EXE)

$(EXE):  main.c naive_scalar.c naive_vector.c vectime.c define.h vectime_fixed_array.c vectime_extra_array.c
	$(CC) $(OPT_FLAGS) $(OMP_FLAGS) $^ -lm  -o $(EXE)

clean:
	rm -f  exe* 

