#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctime>

#define III for (i = 0; i < n1; ++i)
#define JJJ for (j = 0; j < m2; ++j)
#define KKK for (k = 0; k < m1; ++k)
#define MULTI(X,Y,Z) X \
	Y \
	Z \
	c[i][j] += a[i][k]*b[k][j];

using namespace std;

typedef vector<double> Vec;
typedef vector<Vec> Matrix;

void readm(Matrix &a, int &n, int &m, char &c, char *f) {
	
	ifstream file(f, ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.read(&c, 1);
	file.read((char*)&n, sizeof(int));
	file.read((char*)&m, sizeof(int));
	a.resize(n,Vec(m));
	
	if (c == 'f') {
		float x;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j) {
				file.read((char*)&x, sizeof(float));
				a[i][j] = x;
			}
	} else {
		double x;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j) {
				file.read((char*)&x, sizeof(double));
				a[i][j] = x;
			}
	}
	file.close();
	
}

void writem(Matrix &a, int n, int m, char c, char *f) {
	
	ofstream file(f, ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.write(&c, 1);
	file.write((char*)&n, sizeof(int));
	file.write((char*)&m, sizeof(int));
	
	if (c == 'f') {
		float x;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j) {
				x = (float) a[i][j];
				file.write((char*)&x, sizeof(float));
			}
	} else {
		double x;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j) {
				x = (double) a[i][j];
				file.write((char*)&x, sizeof(double));
			}
	}
	file.close();
	
}

int main(int argc, char **argv) {
	
	if (argc < 5) {
		cout << "./main A B C {0..5}" << endl;
		return 0;
	}
	const int r = atoi(argv[4]);
	
	Matrix a;
	int n1, m1;
	char c1;
	readm(a,n1,m1,c1,argv[1]);
	
	Matrix b;
	int n2, m2;
	char c2;
	readm(b,n2,m2,c2,argv[2]);
	
	if (m1 != n2) {
		cout << "Умножение невозможно" << endl;
		return 1;
	}
	
	int i,j,k;
	
	Matrix c(n1,Vec(m2));
	clock_t t = clock();
	switch (r) {
	case 0: MULTI(III,JJJ,KKK) break;
	case 1: MULTI(III,KKK,JJJ) break;
	case 2: MULTI(JJJ,III,KKK) break;
	case 3: MULTI(JJJ,KKK,III) break;
	case 4: MULTI(KKK,III,JJJ) break;
	case 5: MULTI(KKK,JJJ,III) break;
	}
	
	cout << (clock() - (float)t) / CLOCKS_PER_SEC << endl;
	
	if (c1 == 'f' and c2 == 'f') writem(c,n1,m2,'f',argv[3]);
		else writem(c,n1,m2,'d',argv[3]);
	
}
