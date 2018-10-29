#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#define CMP(T1,T2) { T1 x; T2 y; \
		for (int i = 0; i < n; ++i) \
			for (int j = 0; j < m; ++j) { \
				f1.read((char*)&x, sizeof(T1)); \
				f2.read((char*)&y, sizeof(T2)); \
				if (abs(x-y) > epsilon) { \
					cout << '0' << endl; \
					cout << '(' << i << ',' << j << "): " << scientific << x << " != " << y << endl; \
					f1.close(); f2.close(); return 0; } \
			} \
		}

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 3) {
		cout << "./equals A B [epsilon]" << endl;
		return 0;
	}
	
	float epsilon = 1.e-8;
	if (argc >= 4) epsilon = atof(argv[3]);
	
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
	f1.read(&c, 1);
	if ((c != 'f') and (c != 'd')) { cout << "incorrect type of matrix " << argv[1] << endl; return 1; }
	f2.read(&c1, 1);
	if ((c1 != 'f') and (c1 != 'd')) { cout << "incorrect type of matrix" << argv[2] << endl; return 1; }
	int n,m,n1,m1;
	f1.read((char*)&n, sizeof(int));
	f1.read((char*)&m, sizeof(int));
	f2.read((char*)&n1, sizeof(int));
	f2.read((char*)&m1, sizeof(int));
	if (n != n1 or m != m1) {
		cout << '0' << endl;
		cout << "Матрицы разных размеров" << endl;
		return 1;
	}
	
	if (c == 'f') {
		if (c1 == 'f') CMP(float,float)
		else CMP(float,double) }
	else { if (c1 == 'f') CMP(double,float)
		else CMP(double,double) }
	
	cout << '1' << endl;
	f1.close();
	f2.close();
	return 0;
	
}
