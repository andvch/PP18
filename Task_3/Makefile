SHELL := /bin/bash
#MAKEFLAGS += --silent
all: main main_p
main: main.cpp
	mpicxx main.cpp -o main
report: main
	echo -e "#N\tTotal time\tAverage time\tMax time" > report ; \
	for n in 16 32 64 128 256 512 1024 2048; do \
	mpirun -n $$n main 1 100000000 out.txt -n >> report ; \
	done
	gnuplot plotscript
main_p: main_p.cpp
	g++ -pthread main_p.cpp -o main_p
report_p: main_p
	echo -e "#N\tTotal time\tAverage time\tMax time" > report_p ; \
	for n in 16 32 64 128 256 512 1024 2048; do \
	./main_p $$n 1 100000000 out.txt -n >> report_p ; \
	done
