#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {
	
	int rank, size;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	
	if (argc < 4) {
		if (!rank) cout << "./main a b file" << endl;
		MPI_Finalize();
		return 0;
	}
	bool flag = false;
	if (argc > 4) if ((argv[4][0] == '-') and (argv[4][1] == 'n')) flag = true;
	
	int a = atoi(argv[1]), b = atoi(argv[2]), c, d;
	if (a < 2) a = 2;
	if ((b < a) or (b < 2)) {
		if (!rank) cout << "Неверный диапазон" << endl;
		MPI_Finalize();
		return 0;
	}
	
	double time, timeMAX, timeSUM, t0;
	int sb = (int) trunc(sqrt(b));
	
	if (rank) {
		c = (a < sb+1) ? sb+1 : a;
		d = (b - c + 1) / (size-1);
		c += d*(rank-1);
		d = (rank == size-1) ? b : c + d - 1;
		if (d < c) {
			d = 0;
			time = 0;
			MPI_Send(&d, 1, MPI_INT, 0,0, comm);
			MPI_Reduce(&time, &timeMAX, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
			MPI_Reduce(&time, &timeSUM, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
			MPI_Finalize();
			return 0;
		}
//		cout << rank << ") " << c << ' ' << d << endl;
	}
	
	
	char *s = NULL, *p = NULL;
	int *buf, x, n, m, i, j, k ,l, sum = 0;
	
	t0 = MPI_Wtime();
	
	if ((rank) or (a < sb+1) or (size == 1)) {
		if (rank) x = (int) trunc(sqrt(d))-1;
			else if (size == 1) x = b - 1;
				else x = sb-1;
		n = (x%8) ? x/8 + 1 : x/8;
		
		s = new char[n];
		fill_n(s, n, 0xFF);
		if (x%8) s[n-1] ^= (0xFF >> x%8);
		
		for (i = 0; i < n; ++i) {
			if (!(s[i])) continue;
			for (j = 0; j < 8; ++j) {
				if (!((s[i] >> (7-j)) & 1)) continue;
				x = 8*i + j + 2;
				k = i + (j + x)/8;
				l = (j + x)%8;
				while (k < n) {
					s[k] &= ~(1 << (7-l));
					l += x;
					k += l/8;
					l %= 8;
				}
			}
		}
	}
	
	
	if (!rank) {
		
		time = MPI_Wtime() - t0;
		
		FILE *file = fopen(argv[3], "w");
		if (file == NULL) {
			cout << "Error opening file " << argv[3] << endl;
			MPI_Finalize();
			return 1;
		}
		
		if ((a < sb+1) or (size == 1)) {
			i = (a-2)/8;
			s[i] &= (0xFF >> (a-2)%8);
			for (; i < n; ++i) {
				if (!(s[i])) continue;
				for (j = 0; j < 8; ++j) {
					if (!((s[i] >> (7-j)) & 1)) continue;
					x = 8*i + j + 2;
					fprintf(file, "%d ", x);
					++sum;
				}
			}
		}
		
		if (size > 1) {
			x = (b-a)/(size) + 1;
			buf = new int[x];
			
			for (i = 1; i < size; ++i) {
				MPI_Recv(buf, x, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
				MPI_Get_count(&status, MPI_INT, &l);
				if (buf[0] == 0) continue;
				for (j = 0; j < l; ++j) fprintf(file, "%d ", buf[j]);
				sum += l;
			}
			delete[] buf;
		}
		
		fclose(file);
		
		MPI_Reduce(&time, &timeMAX, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
		MPI_Reduce(&time, &timeSUM, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
		
		if (flag) cout << size << "\t" << timeSUM << "\t" << timeSUM / size << "\t" << timeMAX << endl;
			else {
			cout << "Найдено " << sum << " простых чисел" << endl;
			cout << "Вычислительных узлов: " << size << endl;
			cout << "Суммарное время: " << timeSUM << endl;
			cout << "Среднее время: " << timeSUM / size << endl;
			cout << "Максимальное время: " << timeMAX << endl;
		}
		
	} else {
		
		x = d - c + 1;
		m = (x%8) ? x/8 + 1 : x/8;
		
		buf = new int[x/2+1];
		
		p = new char[m];
		fill_n(p, m, 0xFF);
		if (x%8) p[m-1] ^= (0xFF >> x%8);
		
		for (i = 0; i < n; ++i) {
			if (!(s[i])) continue;
			for (j = 0; j < 8; ++j) {
				if (!((s[i] >> (7-j)) & 1)) continue;
				x = 8*i + j + 2;
				l = (c%x) ? (x - c%x) : 0;
				k = l/8;
				l %= 8;
				while (k < m) {
					p[k] &= ~(1 << (7-l));
					l += x;
					k += l/8;
					l %= 8;
				}
			}
		}
		
		time = MPI_Wtime() - t0;
		
		for (i = 0; i < m; ++i) {
			if (!(p[i])) continue;
			for (j = 0; j < 8; ++j) {
				if (!((p[i] >> (7-j)) & 1)) continue;
				buf[sum] = c + 8*i + j;
				++sum;
			}
		}
		
		if (sum) MPI_Send(buf, sum, MPI_INT, 0,0, comm);
			else MPI_Send(&sum, 1, MPI_INT, 0,0, comm);
		MPI_Reduce(&time, &timeMAX, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
		MPI_Reduce(&time, &timeSUM, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
		
		delete[] buf;
		
	}
	
	if (s != NULL) delete[] s;
	if (p != NULL) delete[] p;
	
	MPI_Finalize();
	
	return 0;
	
}
