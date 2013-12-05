all: waterflow.cxx state.h state.cxx
	g++ waterflow.cxx -lncurses -o waterflow
