#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <papi.h>

#define _I for (i = 0; i < n; i += block)
#define _J for (j = 0; j < m; j += block)
#define _K for (k = 0; k < l; k += block)
#define _II for (i1 = i; (i1 < i+block) && (i1 < n); ++i1)
#define _JJ for (j1 = j; (j1 < j+block) && (j1 < m); ++j1)
#define _KK for (k1 = k; (k1 < k+block) && (k1 < l); ++k1)

#define MULTI(A,B,C,D,E,F) A \
	B \
	C \
	D \
	E \
	F \
	c[i1][j1] += a[i1][k1]*b[k1][j1];


using namespace std;

void readm(float** &a, int &n, int &m, char *f) {
	
	ifstream file(f, ios::binary);
	if (!file.is_open()) {
		cout << "Error opening file " << f << endl;
		exit(1);
	}
	
	char c;
	file.read(&c, 1);
	if (c != 'f') {
		cout << "Only type float suported" << endl;
		exit(1);
	}
	file.read((char*)&n, sizeof(int));
	file.read((char*)&m, sizeof(int));
		
	float x;
	a = new float*[n];
	for (int i = 0; i < n; ++i) {
		a[i] = new float[m];
		for (int j = 0; j < m; ++j) {
			file.read((char*)&x, sizeof(float));
			a[i][j] = x;
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

int main(int argc, char **argv) {
	
	if (argc < 4) {
		cout << "./main A B C mode block_size" << endl;
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
	int n,m,l,l1;
	readm(a,n,l,argv[1]);
	readm(b,l1,m,argv[2]);
	
	if (l != l1) {
		cout << "Умножение невозможно" << endl;
		return 1;
	}
	
	int i,j,k,i1,j1,k1;
	c = new float*[n];
	for (i = 0; i < n; ++i) {
		c[i] = new float[m];
		for (j = 0; j < m; ++j) c[i][j] = 0;
	}
	
	if (PAPI_start_counters(Events, 4) != PAPI_OK) {
		cout << "PAPI_error" << endl;
		exit(1);
	}
	clock_t t = clock();
	
	switch (mode) {
		case 1: MULTI(_I,_K,_J,_II,_KK,_JJ) break; \
		case 2: MULTI(_J,_I,_K,_JJ,_II,_KK) break; \
		case 3: MULTI(_J,_K,_I,_JJ,_KK,_II) break; \
		case 4: MULTI(_K,_I,_J,_KK,_II,_JJ) break; \
		case 5: MULTI(_K,_J,_I,_KK,_JJ,_II) break; \
		default: MULTI(_I,_J,_K,_II,_JJ,_KK) break; \
	}
	
	cout << (clock() - (float) t) / CLOCKS_PER_SEC << endl;
	if (PAPI_stop_counters(values, 4) != PAPI_OK) {
		cout << "PAPI_error" << endl;
		exit(1);
	}
	
	writem(c,n,m,argv[3]);
	
	cout << values[0] << endl << values[1] << endl << values[2] << endl << values[3] << endl;
	
	for (i = 0; i < n; ++i) delete a[i]; delete a;
	for (i = 0; i < l; ++i) delete b[i]; delete b;
	for (i = 0; i < n; ++i) delete c[i]; delete c;
	
	return 0;
	
}
