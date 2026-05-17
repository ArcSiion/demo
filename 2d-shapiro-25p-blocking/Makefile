EXE=exe-2d-shapiro-25p-blocking

include ../common.make

all: $(EXE)

SRC=main.c naive_vector.c blocking_parallel_rectangle_scalar.c \
	blocking_parallel_rectangle_vector.c \
	blocking_parallel_rectangle_vectime_extra_array.c

$(EXE): $(SRC) *.h
	$(CC) $(OPT_FLAGS) -mfma -ffp-contract=fast $(OMP_FLAGS) $^ -lm -o $(EXE)

clean:
	rm -f exe*
