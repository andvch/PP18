#include <iostream>
#include <fstream>
#include <iomanip>
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
		case 1: CASES(A,B,C,JJJ,III,KKK) break; \
		case 2: CASES(A,B,C,III,KKK,JJJ) break; \
		case 3: CASES(A,B,C,KKK,III,JJJ) break; \
		case 4: CASES(A,B,C,JJJ,KKK,III) break; \
		case 5: CASES(A,B,C,KKK,JJJ,III) break; \
		}
		
#define DEL(A,N) if (A != NULL) { for (i = 0; i < N; ++i) delete A[i]; delete A; }

using namespace std;

void readm(float** &a, double** &b, int &n, int &m, char &c, char *f) {
	
	ifstream file(f, ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.read(&c, 1);
	file.read((char*)&n, sizeof(int));
	file.read((char*)&m, sizeof(int));
	
	if (c == 'f') {
		b = NULL;
		float x;
		a = new float*[n];
		for (int i = 0; i < n; ++i) {
			a[i] = new float[m];
			for (int j = 0; j < m; ++j) {
				file.read((char*)&x, sizeof(float));
				a[i][j] = x;
			}
		}
	} else {
		a = NULL;
		double x;
		b = new double*[n];
		for (int i = 0; i < n; ++i) {
			b[i] = new double[m];
			for (int j = 0; j < m; ++j) {
				file.read((char*)&x, sizeof(double));
				b[i][j] = x;
			}
		}
	}
	file.close();
	
}

void writem(float** a, int n, int m, char *f) {
	
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
			x = a[i][j];
			file.write((char*)&x, sizeof(float));
		}
	file.close();
	
}

void writem(double** a, int n, int m, char *f) {
	
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
			x = a[i][j];
			file.write((char*)&x, sizeof(double));
		}
	file.close();
	
}

int main(int argc, char **argv) {
	
	if (argc < 5) {
		cout << "./main A B C {0..5}" << endl;
		return 0;
	}
	const int r = atoi(argv[4]);
	
	float **af, **bf, **cf = NULL;
	double **ad, **bd, **cd = NULL;
	int n1,n2,m1,m2;
	char c1,c2;
	readm(af,ad,n1,m1,c1,argv[1]);
	readm(bf,bd,n2,m2,c2,argv[2]);
	
	if (m1 != n2) {
		cout << "Умножение невозможно" << endl;
		return 1;
	}
	
	int i,j,k;
	
	clock_t t = clock();
	if (c1 == 'f' and c2 == 'f') {
		cf = new float*[n1];
		for (i = 0; i < n1; ++i) {
			cf[i] = new float[m2];
			for (j = 0; j < m2; ++j) cf[i][j] = 0;
		}
		MULTI(af,bf,cf)
	} else {
		cd = new double*[n1];
		for (i = 0; i < n1; ++i) {
			cd[i] = new double[m2];
			for (j = 0; j < m2; ++j) cd[i][j] = 0;
		}
		if (c1 == 'f') MULTI(af,bd,cd)
		else if (c2 == 'f') MULTI(ad,bf,cd)
		else MULTI(ad,bd,cd)
	}
	
	cout << (clock() - (float) t) / CLOCKS_PER_SEC << endl;
	
	if (c1 == 'f' and c2 == 'f') writem(cf,n1,m2,argv[3]);
		else writem(cd,n1,m2,argv[3]);
		
	DEL(af,n1)
	DEL(ad,n1)
	DEL(bf,n2)
	DEL(bd,n2)
	DEL(cf,n1)
	DEL(cd,n1)
	
}
