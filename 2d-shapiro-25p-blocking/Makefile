EXE=exe-2d-shapiro-25p-blocking

include ../common.make

all: $(EXE)

$(EXE): *.c *.h
	$(CC) $(OPT_FLAGS) -mfma -ffp-contract=fast $(OMP_FLAGS) $^ -lm -o $(EXE)

clean:
	rm -f exe*
