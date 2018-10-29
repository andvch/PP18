#include <iostream>
#include <fstream>
#include <iomanip>

#define DATA(T) T d; \
	for (int i = 0; i < n; ++i) { \
		for (int j = 0; j < m; ++j) { \
			file.read((char*)&d, sizeof(T)); \
			cout << d << '\t'; \
		} \
	cout << endl; \
	}

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 2) {
		cout << "./print file" << endl;
		return 0;
	}
	
	ifstream file(argv[1], ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		return 1;
	}
	
	char c;
	file.read(&c, 1);
	if ((c != 'f') and (c != 'd')) {cout << "incorrect type of matrix" << endl; return 1;}
	int n,m;
	file.read((char*)&n, sizeof(n));
	file.read((char*)&m, sizeof(m));
	
	cout << n << 'x' << m;
	if (c == 'f') { cout << " (float)" << endl << scientific; DATA(float) }
		else { cout << " (double)" << endl << scientific; DATA(double) }
	
	file.close();
	return 0;
	
}
