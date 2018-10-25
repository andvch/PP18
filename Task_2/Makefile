SHELL := /bin/bash
#MAKEFLAGS += --silent
all: main generate print equals
main: main.cpp
	g++ main.cpp /usr/lib64/libpapi.so -o main -std=c++11
generate: generate.cpp
	g++ generate.cpp -o generate -std=c++11
print: print.cpp
	g++ print.cpp -o print -std=c++11
equals: equals.cpp
	g++ equals.cpp -o equals -std=c++11
report: main
	rm -rf plots
	mkdir plots
	function print { \
		echo -n -e "\t$$1" >> plots/TIME ; \
		echo -n -e "\t$$2" >> plots/L1 ; \
		echo -n -e "\t$$3" >> plots/L2 ; \
		echo -n -e "\t$$4" >> plots/CYC ; \
		echo -n -e "\t$$5" >> plots/FLOP ; \
	} ; \
	echo -e "#size\t1\t2\t3"| tee plots/TIME plots/L1 plots/L2 plots/CYC plots/FLOP > /dev/null ; \
	for way in ./tests/*; do \
		echo -n -e "$${way##*/}" | tee -a plots/TIME plots/L1 plots/L2 plots/CYC plots/FLOP ; \
		print `./main $$way/A.dat $$way/B.dat $$way/D.dat 0` ; \
		print `./main $$way/A.dat $$way/B.dat $$way/D.dat 1` ; \
		print `./main $$way/A.dat $$way/B.dat $$way/D.dat 1 73` ; \
		print `./main $$way/A.dat $$way/B.dat $$way/D.dat 1 209` ; \
#		rm $$way/D.dat ; \
		echo | tee -a plots/TIME plots/L1 plots/L2 plots/CYC plots/FLOP ; \
	done
	gnuplot plotscript
test: main equals
	for way in ./tests/*; do \
		echo "Тестируется $$way" ; \
		for ((i=0;i<2;i++)) do \
			./main $$way/A.dat $$way/B.dat $$way/D.dat $$i > /dev/null; \
			x=`./equals $$way/C.dat $$way/D.dat` ; \
			if [[ $$x != 1 ]]; then \
				echo "Тест не пройден (режим $$i)" ; \
				rm $$way/D.dat ; \
				exit ; \
			fi \
		done ; \
#		rm $$way/D.dat ; \
	done ; \
	echo "Тест пройден"
