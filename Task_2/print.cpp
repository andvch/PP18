#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 2) {
		cout << "./print file [-a]" << endl;
		return 0;
	}
	
	bool a = false;
	if (argc > 2) if ((argv[2][0] == '-') && (argv[2][1] == 'a')) a = true; 
	
	ifstream file(argv[1], ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		return 1;
	}
	
	int n;
	file.read((char*)&n, sizeof(n));
	cout << n << 'x' << n << scientific << endl;
	
	if (!a) { file.close(); return 0; }
	
	float x;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			file.read((char*)&x, sizeof(float));
			cout << x << '\t';
		}
	cout << endl;
	}
	
	file.close();
	return 0;
	
}
