#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	
	if (argc < 3) {
		cout << "./generate N file" << endl;
		return 0;
	}
	
	
	int n = atoi(argv[1]);
	
	if (n <= 0) {
		cout << "incorrect size of matrix" << endl;
		return 0;
	}
	
	ofstream file(argv[2], ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		return 1;
	}
	
	file.write((char*)&n, sizeof(n));
	
	srand(time(0));
	float x;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			x = rand()%200 - 100 + (float)rand()/RAND_MAX;
			file.write((char*)&x, sizeof(float));
		}
	
	file.close();
	return 0;
	
}
