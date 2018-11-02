SHELL := /bin/bash
#MAKEFLAGS += --silent
all: main
main: main.cpp
	mpicxx main.cpp -o main
report: main
	echo -e "#N\tTotal time\tAverage time\tMax time" > report ; \
	for n in 16 32 64 128 256 512 1024 2048; do \
	mpirun -n $$n main 1 100000000 out.txt -n >> report ; \
	done
	gnuplot plotscript
