SHELL := /bin/bash
#MAKEFLAGS += --silent
n := 1024
m := 4096

all: main generate print equals
main: main.cpp
	mpicxx $^ -o $@
generate: generate.cpp
	g++ $^ -o $@
print: print.cpp
	g++ $^ -o $@
equals: equals.cpp
	g++ $^ -o $@
mapgen: mapgen.cpp
	g++ $^ -o $@
A.dat: generate
	./generate d $(n) $(m) $@
B.dat: generate
	./generate d $(m) 1 $@
my.map: mapgen
	./mapgen $@
submit: main A.dat B.dat my.map
	for n in 0001 0032 0064 0128 0256 0512; do \
		mpisubmit.bg -n $$n -w 00:05:00 --stdout time_$$n --stderr $$n.err ./main -- A.dat B.dat C_$$n.dat ; \
	done
	llsubmit job.jcf
report:
	x=(`cat time_0001`) ; \
	n=$${x[3]} ; \
	for file in time_*; do \
		x=(`cat $$file`) ; \
		x[1]=$${x[3]} ; \
		x[2]=`echo "$$n $${x[1]}" | awk '{ print $$1 / $$2 }'` ; \
		x[3]=`echo "$${x[2]} $${x[0]}" | awk '{ print $$1 / $$2 }'` ; \
		echo "$${x[@]}" | sed 's/ /\t/g' >> report ; \
	done
	gnuplot plotscript
clean:
	rm main generate print equals mapgen my.map *.dat time_*
