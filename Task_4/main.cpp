#include <iostream>
#include <fstream>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
	
	int rank, size;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	
	if (argc < 4) {
		if (!rank) cout << "./main A B C" << endl;
		MPI_Finalize();
		return 0;
	}
	
	MPI_File fa, fb, fc;
	double *a, *b, *c, *s;
	int n[2],m[2],l[2], i, j, p;
	char ca, cb;
	
	if (MPI_File_open(comm, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fa)) {
		if (!rank) cout << "Error opening file " << argv[1] << endl;
		MPI_Finalize();
		return 1;
	}
	if (MPI_File_open(comm, argv[2], MPI_MODE_RDONLY, MPI_INFO_NULL, &fb)) {
		if (!rank) cout << "Error opening file " << argv[2] << endl;
		MPI_Finalize();
		return 1;
	}
	
	MPI_File_read(fa, &ca, 1, MPI_CHAR, MPI_STATUS_IGNORE);
	MPI_File_read(fb, &cb, 1, MPI_CHAR, MPI_STATUS_IGNORE);
	if ((ca != 'd') or (cb != 'd')) {
		if (!rank) cout << "Only type double suported" << endl;
		MPI_Finalize();
		return 1;
	}
	MPI_File_read(fa, n, 2, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_read(fb, m, 2, MPI_INT, MPI_STATUS_IGNORE);
	if (n[1] != m[0]) {
		if (!rank) cout << "Умножение невозможно" << endl;
		MPI_Finalize();
		return 1;
	}
	if (m[1] != 1) {
		if (!rank) cout << argv[2] << " не является вектором" << endl;
		MPI_Finalize();
		return 1;
	}
	
	bool mode = (n[0] > n[1]);
	if (mode) {
		
		l[0] = n[0] / size;
		l[1] = n[1];
		MPI_File_seek(fa,sizeof(double)*rank*l[0]*l[1], MPI_SEEK_CUR);
		if (rank == size-1) l[0] += n[0] % size;
		a = new double[l[0]*l[1]];
		MPI_File_read(fa, a, l[0]*l[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
		
		b = new double[l[1]];
		MPI_File_read(fb, b, l[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
		
	} else {
		
		l[0] = n[0];
		l[1] = n[1] / size;
		MPI_File_seek(fa,sizeof(double)*rank*l[1], MPI_SEEK_CUR);
		MPI_File_seek(fb,sizeof(double)*rank*l[1], MPI_SEEK_CUR);
		if (rank == size-1) l[1] += n[1] % size;
		a = new double[l[0]*l[1]];
		for (i = 0; i < n[0]; ++i) {
			MPI_File_read(fa, a+i*l[1], l[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
			MPI_File_seek(fa, sizeof(double)*(n[1]-l[1]), MPI_SEEK_CUR);
		}
		
		b = new double[l[1]];
		MPI_File_read(fb, b, l[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
		
	}
	MPI_File_close(&fa);
	MPI_File_close(&fb);
	
	c = new double[n[0]];
	fill_n(c, n[0], 0);
	p = (mode) ? rank*(n[0]/size) : 0;
	
	double time, timeMAX, timeSUM, t0 = MPI_Wtime();
	
	for (i = 0; i < l[0]; ++i)
		for (j = 0; j < l[1]; ++j)
			c[p+i] += a[i*l[1]+j]*b[j];
	
	time = MPI_Wtime() - t0;
	
	if (!rank) s = new double[n[0]];
	MPI_Reduce(c, s, n[0], MPI_DOUBLE, MPI_SUM, 0, comm);
	MPI_Reduce(&time, &timeMAX, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
	MPI_Reduce(&time, &timeSUM, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
	if (!rank) {
		
		ofstream fc(argv[3], ios::binary | ios::out);
		if (!fc.is_open()) {
			cout << "Error opening file " << argv[3] << endl;
			MPI_Finalize();
			return 1;
		}
		fc.write("d", 1);
		fc.write((char*)n, sizeof(int));
		i = 1;
		fc.write((char*)&i, sizeof(int));
		for (int i = 0; i < n[0]; ++i) fc.write((char*)(s+i), sizeof(double));
		fc.close();
		
		cout << size << "\t" << timeSUM << "\t" << timeSUM / size << "\t" << timeMAX;
		if (argc > 4) cout << "\t" << argv[4];
		cout << endl;
		
	}
	
	delete[] a;
	delete[] b;
	delete[] c;
	if (!rank) delete[] s;
	
	MPI_Finalize();
	return 0;
	
}
