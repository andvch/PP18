#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#define epsilon 0.000000000000001

#define CMP(T) { T x,y; \
		for (int i = 0; i < n; ++i) \
			for (int j = 0; j < m; ++j) { \
				f1.read((char*)&x, sizeof(T)); \
				f2.read((char*)&y, sizeof(T)); \
				if (abs(x-y) > epsilon) { cout << '0' << endl; \
					f1.close(); f2.close(); return 0; } \
			} \
		}

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 3) {
		cout << "./equals A B" << endl;
		return 0;
	}
	
	ifstream f1(argv[1], ios::binary), f2(argv[2], ios::binary);
	if (!f1.is_open()) {
		cout << "Error opening file " << argv[1] << endl;
		return 1;
	}
	if (!f2.is_open()) {
		cout << "Error opening file " << argv[2] << endl;
		return 1;
	}
	
	char c,c1;
	int n,m,n1,m1;
	f1.read(&c, 1);
	f1.read((char*)&n, sizeof(int));
	f1.read((char*)&m, sizeof(int));
	f2.read(&c1, 1);
	f2.read((char*)&n1, sizeof(int));
	f2.read((char*)&m1, sizeof(int));
	if (c != c1 or n != n1 or m != m1) {
		cout << "Матрицы разных форматов" << endl;
		return 0;
	}
	if (c == 'f') CMP(float)
		else CMP(double)
	cout << '1' << endl;
	f1.close();
	f2.close();
}
