#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <ctime>

#define III for (i = 0; i < n1; ++i)
#define JJJ for (j = 0; j < m2; ++j)
#define KKK for (k = 0; k < m1; ++k)
#define CASES(A,B,C,X,Y,Z) X \
	Y \
	Z \
	C[i][j] += A[i][k]*B[k][j];
	
#define MULTI(A,B,C) switch (r) { \
		case 0: CASES(A,B,C,III,JJJ,KKK) break; \
		case 1: CASES(A,B,C,III,KKK,JJJ) break; \
		case 2: CASES(A,B,C,JJJ,III,KKK) break; \
		case 3: CASES(A,B,C,JJJ,KKK,III) break; \
		case 4: CASES(A,B,C,KKK,III,JJJ) break; \
		case 5: CASES(A,B,C,KKK,JJJ,III) break; \
		}

using namespace std;

typedef vector<float> VecF;
typedef vector<VecF> MatrixF;
typedef vector<double> VecD;
typedef vector<VecD> MatrixD;

void readm(MatrixF &a, MatrixD &b, int &n, int &m, char &c, char *f) {
	
	ifstream file(f, ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.read(&c, 1);
	file.read((char*)&n, sizeof(int));
	file.read((char*)&m, sizeof(int));
	a.resize(n,VecF(m));
	b.resize(n,VecD(m));
	
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
				b[i][j] = x;
			}
	}
	file.close();
	
}

void writem(MatrixF &a, int n, int m, char *f) {
	
	ofstream file(f, ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.write("f", 1);
	file.write((char*)&n, sizeof(int));
	file.write((char*)&m, sizeof(int));
	
	float x;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			x = (float) a[i][j];
			file.write((char*)&x, sizeof(float));
		}
	file.close();
	
}

void writem(MatrixD &a, int n, int m, char *f) {
	
	ofstream file(f, ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.write("d", 1);
	file.write((char*)&n, sizeof(int));
	file.write((char*)&m, sizeof(int));
	
	double x;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			x = (double) a[i][j];
			file.write((char*)&x, sizeof(float));
		}
	file.close();
	
}

int main(int argc, char **argv) {
	
	if (argc < 5) {
		cout << "./main A B C {0..5}" << endl;
		return 0;
	}
	const int r = atoi(argv[4]);
	
	MatrixF af;
	MatrixD ad;
	int n1, m1;
	char c1;
	readm(af,ad,n1,m1,c1,argv[1]);
	
	MatrixF bf;
	MatrixD bd;
	int n2, m2;
	char c2;
	readm(bf,bd,n2,m2,c2,argv[2]);
	
	if (m1 != n2) {
		cout << "Умножение невозможно" << endl;
		return 1;
	}
	
	int i,j,k;
	
	MatrixF cf;
	MatrixD cd;
	clock_t t;
	
	if (c1 == 'f' and c2 == 'f') {
		cf.resize(n1,VecF(m2));
		t = clock();
		MULTI(af,bf,cf)
	} else {
		cd.resize(n1,VecD(m2));
		if (c1 == 'f') {
			t = clock();
			MULTI(af,bd,cd)
		} else if (c2 == 'f') {
			t = clock();
			MULTI(ad,bf,cd)
		} else {
			t = clock();
			MULTI(ad,bd,cd)
		} 
	}
	cout << (clock() - (float) t) / CLOCKS_PER_SEC << endl;
	
	if (c1 == 'f' and c2 == 'f') writem(cf,n1,m2,argv[3]);
		else writem(cd,n1,m2,argv[3]);
	
}
