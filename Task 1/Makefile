SHELL := /bin/bash
MAKEFLAGS += --silent
all: main generate print equals
main: main.cpp
	g++ main.cpp -o main -std=c++11
generate: generate.cpp
	g++ generate.cpp -o generate -std=c++11
print: print.cpp
	g++ print.cpp -o print -std=c++11
equals: equals.cpp
	g++ equals.cpp -o equals -std=c++11
report: generate main
	echo -e "#X\tY" > report
	n=10 ; \
	t0=0.0 ; t1=0.0 ; t2=0.0 ; t3=0.0 ; t4=0.0 ; t5=0.0 ; \
	for ((i=0;i<$$n;i++)) do \
		echo -n "$$i " ; \
		./generate f 500 500 A.dat ; \
		./generate f 500 500 B.dat ; \
		x='' ; \
		for ((j=0;j<=5;j++)) do \
			x+=`./main A.dat B.dat C.dat $$j`' ' ; \
		done ; \
		t0=`echo "$$t0 $$x" | awk '{ print $$1 + $$2 }'` ; \
		t1=`echo "$$t1 $$x" | awk '{ print $$1 + $$3 }'` ; \
		t2=`echo "$$t2 $$x" | awk '{ print $$1 + $$4 }'` ; \
		t3=`echo "$$t3 $$x" | awk '{ print $$1 + $$5 }'` ; \
		t4=`echo "$$t4 $$x" | awk '{ print $$1 + $$6 }'` ; \
		t5=`echo "$$t5 $$x" | awk '{ print $$1 + $$7 }'` ; \
	done ; \
	echo -e "0\t`echo "$$t0 $$n" | awk '{ print $$1 / $$2 }'`" >> report ; \
	echo -e "1\t`echo "$$t1 $$n" | awk '{ print $$1 / $$2 }'`" >> report ; \
	echo -e "2\t`echo "$$t2 $$n" | awk '{ print $$1 / $$2 }'`" >> report ; \
	echo -e "3\t`echo "$$t3 $$n" | awk '{ print $$1 / $$2 }'`" >> report ; \
	echo -e "4\t`echo "$$t4 $$n" | awk '{ print $$1 / $$2 }'`" >> report ; \
	echo -e "5\t`echo "$$t5 $$n" | awk '{ print $$1 / $$2 }'`" >> report ; \
	rm A.dat ; rm B.dat ; rm C.dat
	gnuplot plotscript
	echo "Готово"
test: main equals
	for way in ./tests/*; do \
		echo "Тестируется $$way" ; \
		for ((i=0;i<5;i++)) do \
			./main $$way/A.dat $$way/B.dat $$way/D.dat $$i > /dev/null; \
			x=`./equals $$way/C.dat $$way/D.dat` ; \
			if [[ $$x != 1 ]]; then \
				echo "Тест не пройден" ; \
				rm $$way/D.dat ; \
				exit ; \
			fi \
		done ; \
		rm $$way/D.dat ; \
	done ; \
	echo "Тест пройден"
