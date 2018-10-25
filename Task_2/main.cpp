#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <papi.h>

#define MULTI(X,Y,Z,S) \
		for (X = 0; X < n; X += S) \
		for (Y = 0; Y < n; Y += S) \
		for (Z = 0; Z < n; Z += S) \
		for (X##1 = X; (X##1 < X+S) && (X##1 < n); X##1++) \
		for (Y##1 = Y; (Y##1 < Y+S) && (Y##1 < n); Y##1++) \
		for (Z##1 = Z; (Z##1 < Z+S) && (Z##1 < n); Z##1++) \
		c[i1][j1] += a[i1][k1]*b[k1][j1];


using namespace std;

void readm(float** &m, int &n, char *f) {
	
	ifstream file(f, ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.read((char*)&n, sizeof(int));
		
	float x;
	m = new float*[n];
	for (int i = 0; i < n; ++i) {
		m[i] = new float[n];
		for (int j = 0; j < n; ++j) {
			file.read((char*)&x, sizeof(float));
			m[i][j] = x;
		}
	}
	file.close();
	
}

void writem(float** m, int n, char *f) {
	
	ofstream file(f, ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	file.write((char*)&n, sizeof(int));
		
	float x;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			x = m[i][j];
			file.write((char*)&x, sizeof(float));
		}
	file.close();
	
}

int main(int argc, char **argv) {
	
	if (argc < 4) {
		cout << "./main A B C mode block" << endl;
		return 0;
	}
	char mode = 0;
	if (argc >= 5) mode = atoi(argv[4]);
	int block = 32;
	if (argc >= 6) block = atoi(argv[5]);
	
	/* Initialize the library */
	if (PAPI_VER_CURRENT != PAPI_library_init(PAPI_VER_CURRENT)) {
		cout << "PAPI_library_init error." << endl;
		exit(1);
	}
	if (PAPI_is_initialized() != PAPI_LOW_LEVEL_INITED)  {
		cout << "PAPI_library_init error." << endl;
		exit(1);
	}
	
	long long values[4];
	fill_n(values, 4, 0);
	int Events[]={PAPI_L1_DCM,PAPI_L2_DCM,PAPI_TOT_CYC,PAPI_FP_OPS};
		
	float **a, **b, **c;
	int n,m;
	readm(a,n,argv[1]);
	readm(b,m,argv[2]);
	
	if (n != m) {
		cout << "Умножение невозможно" << endl;
		return 1;
	}
	
	int i,j,k,i1,j1,k1;
	c = new float*[n];
	for (i = 0; i < n; ++i) {
		c[i] = new float[n];
		for (j = 0; j < n; ++j) c[i][j] = 0;
	}
	
	if (PAPI_start_counters(Events, 4) != PAPI_OK) {
		cout << "PAPI_error" << endl;
		exit(1);
	}
	clock_t t = clock();
	
	if (mode) MULTI(i,k,j,block)
		else MULTI(i,j,k,block)
	
	cout << (clock() - (float) t) / CLOCKS_PER_SEC << endl;
	if (PAPI_stop_counters(values, 4) != PAPI_OK) {
		cout << "PAPI_error" << endl;
		exit(1);
	}
	
	writem(c,n,argv[3]);
	
	cout << values[0] << endl << values[1] << endl << values[2] << endl << values[3] << endl;
	
	for (i = 0; i < n; ++i) delete a[i]; delete a;
	for (i = 0; i < n; ++i) delete b[i]; delete b;
	for (i = 0; i < n; ++i) delete c[i]; delete c;
	
	return 0;
	
}
