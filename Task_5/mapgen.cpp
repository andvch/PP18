#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 2) {
		cout << argv[0] << " file" << endl;
		return 0;
	}
	ofstream file(argv[1], ios::out);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		return 1;
	}
	
	int n = 128;
	int a[n], i, j, t;
	for (i = 0; i < n; ++i) a[i] = i;
	
	srand(time(0));
	for (i = n-1; i > 0; --i) {
		 j = rand()%(i+1);
		 t = a[i];
		 a[i] = a[j];
		 a[j] = t;
	}
	for (i = 0; i < n; ++i) file << a[i] / 32 << ' ' << (a[i] % 32) / 8 << ' ' << a[i] % 8 << " 0" << endl;
	
	file.close();
	return 0;
	
}
