#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 3) {
		cout << "./equals A B epsilon" << endl;
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
	
	int n,m;
	f1.read((char*)&n, sizeof(int));
	f2.read((char*)&m, sizeof(int));
	if (n != m) {
		cout << '0' << endl;
		cout << "Матрицы разных размеров" << endl;
		return 0;
	}
	float x,y;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			f1.read((char*)&x, sizeof(float));
			f2.read((char*)&y, sizeof(float));
			if (abs(x-y) > epsilon) {
				cout << '0' << endl;
				cout << i << ' ' << j << ": " << scientific << x << ' ' << y << endl;
				f1.close();
				f2.close();
				return 0;
			}
		}
	cout << '1' << endl;
	f1.close();
	f2.close();
	return 0;
}
