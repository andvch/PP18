#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#define DATA(T) { T d; \
	for (int i = 0; i < n; ++i) { \
		for (int j = 0; j < m; ++j) { \
			d = rand()%200 - 100 + (T)(rand())/RAND_MAX; \
			file.write((char*)&d, sizeof(T)); \
		} \
	} \
} 
	

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 5) {
		cout << "./generate f 10 10 file" << endl;
		return 0;
	}
	
	if (argv[1][0] != 'f' and argv[1][0] != 'd') {
		cout << "incorrect type" << endl << "f - float" << endl  << "d - double" << endl;
		return 0;
	}
	
	int n = atoi(argv[2]), m = atoi(argv[3]);
	
	if (n == 0 or m == 0) {
		cout << "incorrect size of matrix" << endl;
		return 0;
	}
	
	ofstream file(argv[4], ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		return 1;
	}
	
	file.write(argv[1], 1);
	file.write((char*)&n, sizeof(n));
	file.write((char*)&m, sizeof(m));
		
	srand(time(0));
	if (argv[1][0] == 'f') DATA(float)
		else DATA(double)
	
	file.close();
	
}
