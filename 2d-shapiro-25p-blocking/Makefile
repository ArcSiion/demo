EXE=exe-2d-shapiro-25p-blocking
BIG_EXE=exe-2d-shapiro-25p-blocking-big

include ../common.make

all: $(EXE)

SRC=main.c naive_vector.c blocking_parallel_rectangle_scalar.c \
	blocking_parallel_rectangle_vector.c \
	blocking_parallel_rectangle_vectime_extra_array.c

BIG_SRC=main_big.c blocking_parallel_rectangle_scalar.c \
	blocking_parallel_rectangle_vectime_extra_array.c

$(EXE): $(SRC) *.h
	$(CC) $(OPT_FLAGS) -mfma -ffp-contract=fast $(OMP_FLAGS) $^ -lm -o $(EXE)

big: $(BIG_EXE)

$(BIG_EXE): $(BIG_SRC) *.h
	$(CC) $(OPT_FLAGS) -mfma -ffp-contract=fast $(OMP_FLAGS) $^ -lm -o $(BIG_EXE)

clean:
	rm -f exe*
