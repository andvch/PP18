SHELL := /bin/bash
MAKEFLAGS += --silent
all: main.o generate.o print.o equals.o
	g++ -o main main.o
	g++ -o generate generate.o
	g++ -o print print.o
	g++ -o equals equals.o
main.o: main.cpp
	g++ -c -o main.o main.cpp -std=c++11
	echo "main.cpp обновлён"
generate.o: generate.cpp
	g++ -c -o generate.o generate.cpp -std=c++11
	echo "generate.cpp обновлён"
print.o: print.cpp
	g++ -c -o print.o print.cpp -std=c++11
	echo "print.cpp обновлён"
equals.o: equals.cpp
	g++ -c -o equals.o equals.cpp -std=c++11
	echo "equals.cpp обновлён"
report: generate main
	./generate d 500 100 A
	./generate d 100 500 B
	echo -e "#X\tY" > report ; \
	n=10 ; \
	for ((i=0;i<=5;i++)) do \
		echo "Режим $$i..." ; \
		echo -n -e "$$i\t" >> report ; \
		time=`./main A.dat B.dat C.dat $$i` ; \
		for ((j=1;j<$$n;j++)) do \
			x=`./main A.dat B.dat C.dat $$i` ; \
			time=`echo "$$time $$x" | awk '{ print $$1 + $$2 }'` ; \
		done ; \
		time=`echo "$$time $$n" | awk '{ print $$1 / $$2 }'` ; \
		echo $$time >> report; \
	done
	gnuplot plotscript
	echo "Готово"
test: main
	for way in ./tests/*; do \
		echo "Тестируется $$way" ; \
		for ((i=0;i<5;i++)) do \
			./main $$way/A.dat $$way/B.dat $$way/D.dat $$i > /dev/null; \
			x=`./equals $$way/C.dat $$way/D.dat` ; \
			if [[ $$x != 1 ]]; then \
				echo "Тест не пройден" ; \
				rm $$way/D.dat ; \
				exit 0 ; \
			fi \
		done ; \
		rm $$way/D.dat ; \
	done ; \
	echo "Тест пройден"
