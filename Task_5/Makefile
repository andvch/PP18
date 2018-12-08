SHELL := /bin/bash
#MAKEFLAGS += --silent
n := 1024

all: main generate print equals mapgen
main: main.cpp
	mpicxx $^ -o $@ -O3
.cpp:
	g++ $^ -o $@
%.dat: generate
	./generate d $(n) $(n) $@
%.map: mapgen
	./mapgen $@
submit: main A.dat B.dat my.map
	for n in 0001 0008 0064 0125; do \
		mpisubmit.bg -n $$n -w 00:05:00 --stdout time_$$n --stderr $$n.err ./main -- A.dat B.dat C_$$n.dat ; \
	done
	llsubmit job.jcf
.PHONY: report
report:
	x=(`cat time_0001`) ; \
	cat time_* | awk -v t1="$${x[2]}" -v t2="$${x[4]}" -v t3="$${x[6]}" \
		'{ print $$1 "\t" $$5 "\t" t2 / $$5 "\t" t2 / ($$1*$$5) "\t" $$3 "\t" $$7 "\t" $$8 }' > report ; \
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
	rm main generate print equals mapgen *.map *.dat time_*
