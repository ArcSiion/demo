EXE=exe-2d-shapiro-25p

include ../common.make

all: $(EXE)

$(EXE): main.c naive_scalar.c naive_vector.c vectime_transpose_boundary_extra_array.c define.h
	$(CC) $(OPT_FLAGS) -mfma -ffp-contract=fast $(OMP_FLAGS) $^ -lm  -o $(EXE)

clean:
	rm -f  exe* 
