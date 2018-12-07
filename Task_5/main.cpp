#include <iostream>
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
		if (!rank) cout << argv[0] << " A B C" << endl;
		MPI_Finalize();
		return 0;
	}
	
	int p = 1;
	while (p*p*p < size) ++p;
	if (p*p*p != size) {
		if (!rank) cout << size << " != n^3" << endl;
		MPI_Finalize();
		return 0;
	}
	int p2 = p*p;
	
	MPI_File fa, fb, fc;
	double *a, *b, *c, *s;
	int n[2], m[2], i, j, k;
	char ca, cb;
	
	MPI_Group MPI_GROUP_WORLD,  group0, groupA, groupB, groupC;
	MPI_Comm comm0, commA, commB, commC;
	MPI_Comm_group(comm, &MPI_GROUP_WORLD);
	int *process_ranks = new int[p2];
	
	for (i = 0; i < p2; ++i) process_ranks[i] = i;
	MPI_Group_incl(MPI_GROUP_WORLD, p2, process_ranks, &group0);
	MPI_Comm_create(comm, group0, &comm0);
	
	k = rank - rank % p2 + rank % p;
	for (i = 0; i < p; ++i) process_ranks[i] = k + i*p;
	MPI_Group_incl(MPI_GROUP_WORLD, p, process_ranks, &groupA);
	MPI_Comm_create(comm, groupA, &commA);
	
	k = rank - rank % p;
	for (i = 0; i < p; ++i) process_ranks[i] = k + i;
	MPI_Group_incl(MPI_GROUP_WORLD, p, process_ranks, &groupB);
	MPI_Comm_create(comm, groupB, &commB);
	
	k = rank % p2;
	for (i = 0; i < p; ++i) process_ranks[i] = k + i*p2;
	MPI_Group_incl(MPI_GROUP_WORLD, p, process_ranks, &groupC);
	MPI_Comm_create(comm, groupC, &commC);
	
	delete[] process_ranks;
	
	
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
	if (n[0] != n[1]) {
		if (!rank) cout << argv[1] << " не квадратная матрица" << endl;
		MPI_Finalize();
		return 1;
	}
	if (m[0] != m[1]) {
		if (!rank) cout << argv[2] << " не квадратная матрица" << endl;
		MPI_Finalize();
		return 1;
	}
	
	m[1] = m[0] = n[1] = n[0] / p;
	if (rank/p2 == p-1) n[1] += n[0] % p;
	if (rank%p == p-1) m[0] += n[0] % p;
	if (rank%p2/p == p-1) m[1] += n[0] % p;
	a = (n[1] > m[1]) ? new double[m[0]*n[1]] : new double[m[0]*m[1]];
	b = (n[1] > m[0]) ? new double[n[1]*m[1]] : new double[m[0]*m[1]];
	c = new double[m[0]*m[1]];
	fill_n(c, m[0]*m[1], 0);
	
	if (!(rank/p2)) {
		
		MPI_File_seek(fa,sizeof(double)*(rank%p*n[0] + rank/p)*(n[0]/p), MPI_SEEK_CUR);
		MPI_File_seek(fb,sizeof(double)*(rank%p*n[0] + rank/p)*(n[0]/p), MPI_SEEK_CUR);
		for (i = 0; i < m[0]; ++i) {
			MPI_File_read(fa, a+i*m[1], m[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
			MPI_File_seek(fa, sizeof(double)*(n[0]-m[1]), MPI_SEEK_CUR);
			MPI_File_read(fb, b+i*m[1], m[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
			MPI_File_seek(fb, sizeof(double)*(n[0]-m[1]), MPI_SEEK_CUR);
		}
		
		s = new double[m[0]*m[1]];
		
	}
	
	MPI_File_close(&fa);
	MPI_File_close(&fb);
	
	MPI_Barrier(comm);
	double timeMAX, timeSUM, time = MPI_Wtime();
	
	if (rank / p2) { if ((rank % p2) / p == rank / p2) MPI_Recv(a, m[0]*n[1], MPI_DOUBLE, rank % p2, 1, comm, MPI_STATUS_IGNORE); }
		else if (rank / p) MPI_Send(a, m[0]*m[1], MPI_DOUBLE, (rank / p)*p2 + rank, 1, comm);
	
	if (rank / p2) { if (rank % p == rank / p2) MPI_Recv(b, n[1]*m[1], MPI_DOUBLE, rank % p2, 2, comm, MPI_STATUS_IGNORE); }
		else if (rank % p) MPI_Send(b, m[0]*m[1], MPI_DOUBLE, (rank % p)*p2 + rank, 2, comm);
	
	MPI_Bcast(a, m[0]*n[1], MPI_DOUBLE, rank / p2, commA);
	MPI_Bcast(b, n[1]*m[1], MPI_DOUBLE, rank / p2, commB);
	
	for (i = 0; i < m[0]; ++i)
		for (j = 0; j < m[1]; ++j)
			for (k = 0; k < n[1]; ++k)
				c[i*m[1]+j] += a[i*n[1]+k]*b[k*m[1]+j];
	
	MPI_Reduce(c, s, m[0]*m[1], MPI_DOUBLE, MPI_SUM, 0, commC);
	
	time = MPI_Wtime() - time;
	
	delete[] a;
	delete[] b;
	delete[] c;
	
	if (!(rank/p2)) {
		
		if (MPI_File_open(comm0, argv[3], MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fc)) {
			if (!rank) cout << "Error opening file " << argv[3] << endl;
			MPI_Finalize();
			return 1;
		}
		
		if (!rank) {
			n[1] = n[0];
			MPI_File_write(fc, &ca, 1, MPI_CHAR, MPI_STATUS_IGNORE);
			MPI_File_write(fc, n, 2, MPI_INT, MPI_STATUS_IGNORE);
		}
		
		MPI_Datatype block;
		MPI_Type_vector(m[0], m[1], n[0], MPI_DOUBLE, &block);
		MPI_Type_commit(&block);
		MPI_File_set_view(fc, sizeof(char) + 2*sizeof(int) + sizeof(double)*(rank%p*n[0] + rank/p)*(n[0]/p), MPI_DOUBLE, block, "native", MPI_INFO_NULL);
		MPI_File_write(fc, s, m[0]*m[1], MPI_DOUBLE, MPI_STATUS_IGNORE);
		MPI_File_close(&fc);
		MPI_Type_free(&block);
		
		delete[] s;
		
	}
	
	MPI_Reduce(&time, &timeMAX, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
	MPI_Reduce(&time, &timeSUM, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
	
	if (!rank) {
		
		cout << size << "\t" << timeSUM << "\t" << timeSUM / size << "\t" << timeMAX;
		if (argc > 4) cout << "\t" << argv[4];
		cout << endl;
		
	}
	
	MPI_Finalize();
	return 0;
	
}
