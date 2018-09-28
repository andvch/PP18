all: main.o generate.o print.o equals.o
	g++ -o main main.o
	g++ -o generate generate.o
	g++ -o print print.o
	g++ -o equals equals.o
main.o: main.cpp
	g++ -c -o main.o main.cpp -std=c++11
generate.o: generate.cpp
	g++ -c -o generate.o generate.cpp -std=c++11
print.o: print.cpp
	g++ -c -o print.o print.cpp -std=c++11
equals.o: equals.cpp
	g++ -c -o equals.o equals.cpp -std=c++11
report: generate
	./generate f 500 500 A
	./generate f 500 500 B
	echo "0 \c" > report
	./main A B C 0 >> report
	echo "1 \c" >> report
	./main A B C 1 >> report
	echo "2 \c" >> report
	./main A B C 2 >> report
	echo "3 \c" >> report
	./main A B C 3 >> report
	echo "4 \c" >> report
	./main A B C 4 >> report
	echo "5 \c" >> report
	./main A B C 5 >> report
	gnuplot plotscript
