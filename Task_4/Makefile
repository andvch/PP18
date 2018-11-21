SHELL := /bin/bash
#MAKEFLAGS += --silent
nodes := 0032 0064 0128 0256 0512
n := 4096
m := 1024

all: main generate print equals
main: main.cpp
	mpicxx $^ -o $@
generate: generate.cpp
	g++ $^ -o $@
print: print.cpp
	g++ $^ -o $@
equals: equals.cpp
	g++ $^ -o $@
A.dat: generate
	./generate d $(n) $(m) $@
B.dat: generate
	./generate d $(m) 1 $@
run: main A.dat B.dat
	mpisubmit.bg -n 1 --stdout 0001.out --stderr 0001.err ./main -- A.dat B.dat C.dat
	for n in $(nodes); do \
		sed -e "s/num/$$n/g" template.jcf > job.jcf ; \
		llsubmit job.jcf ; \
	done ; \
	x=(XZYT YZXT YXZT ZXYT ZYXT TXYZ TXZY TYZX TYXZ TZXY TZYX) ; \
	x=$${x[$$(($$RANDOM%11))]} ; \
	sed -e "s/num/$$n/g; s/XYZT/$$x/g" template.jcf > job.jcf ; \
	llsubmit job.jcf
	rm job.jcf
report:
	for file in *.out; do cat $$file >> report; done
	rm *.out
