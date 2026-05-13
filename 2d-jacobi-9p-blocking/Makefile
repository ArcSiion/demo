

EXE=exe-2d-jacobi-9p-blocking

include ../common.make

all: $(EXE)

$(EXE):  *.c *.h
	$(CC) $(OPT_FLAGS) $(OMP_FLAGS) $^ -lm -o $(EXE)

clean:
	rm -f  exe* 


