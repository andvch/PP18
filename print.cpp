#include <iostream>
#include <fstream>
#include <iomanip>

#define DATA(T) { T d; \
	for (int i = 0; i < n; ++i) { \
		for (int j = 0; j < m; ++j) { \
			file.read((char*)&d, sizeof(T)); \
			cout << scientific << d << '\t'; \
		} \
	cout << endl; \
	} \
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
	int n,m;
	file.read(&c, 1);
	cout << c << endl;
	file.read((char*)&n, sizeof(n));
	file.read((char*)&m, sizeof(m));
	
	if (c == 'f') DATA(float)
		else DATA(double)
	
	file.close();
	
}
