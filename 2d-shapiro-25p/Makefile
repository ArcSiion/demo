EXE=exe-2d-shapiro-25p

include ../common.make

all: $(EXE)

$(EXE): main.c naive_scalar.c naive_vector.c \
	vectime_transpose_boundary_extra_array.c \
	vectime_transpose_boundary_extra_array_512.c define.h define_512.h
	$(CC) $(OPT_FLAGS) -mavx512f -mfma -ffp-contract=fast $(OMP_FLAGS) $^ -lm  -o $(EXE)

clean:
	rm -f  exe* 
