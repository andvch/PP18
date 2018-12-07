SHELL := /bin/bash
#MAKEFLAGS += --silent
n := 1024

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
A.dat B.dat: generate
	./generate d $(n) $(n) $@
my.map: mapgen
	./mapgen $@
submit: main A.dat B.dat my.map
	for n in 0001 0008 0064 0125; do \
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
test: main equals
	for way in ./tests/*; do \
		echo "Тестируется $$way" ; \
		for n in 1 8 27; do \
			mpirun -n $$n ./main $$way/A.dat $$way/B.dat $$way/D.dat > /dev/null ; \
			x=`./equals $$way/C.dat $$way/D.dat` ; \
			if [[ $$x != 1 ]]; then \
				echo "Тест не пройден (процессов: $$n)" ; \
				rm $$way/D.dat ; \
				exit ; \
			fi \
		done ; \
		rm $$way/D.dat ; \
	done ; \
	echo "Тест пройден"
clean:
	rm main generate print equals mapgen my.map *.dat time_*
