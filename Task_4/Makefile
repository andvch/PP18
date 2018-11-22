SHELL := /bin/bash
#MAKEFLAGS += --silent
nodes := 0032 0064 0128 0256 0512
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
A.dat: generate
	./generate d $(n) $(m) $@
B.dat: generate
	./generate d $(m) 1 $@
submit: main A.dat B.dat
	mpisubmit.bg -n 1 -w 00:01:00 --stdout 0001.out --stderr 0001.err ./main -- A.dat B.dat C.dat
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
	x=(`cat 0001.out`) ; \
	rm 0001.out ; \
	n=$${x[2]} ; \
	for file in *.out; do \
		x=(`cat $$file`) ; \
		x[1]=`echo "$$n $${x[2]}" | awk '{ print $$1 / $$2 }'` ; \
		echo "$${x[@]}" | sed 's/ /\t/g' >> report ; \
	done
	rm *.out
	gnuplot plotscript
