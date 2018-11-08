#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <pthread.h>

using namespace std;

typedef struct arg {
	
	int c, d, n;
	char *s;
	FILE *file;
	float time;
	
} arg;

//pthread_mutex_t lock;

void *work (void *ptr) {
	
	arg *A = (arg *)ptr;
	
	int c = A->c, d = A->d;
	if (d < c) {
		A->time = 0;
		A->n = 0;
		return NULL;
	}
	int x, n = A->n, m, i, j, k, l, sum = 0;
	char *s = A->s, *p;
	
	x = d - c + 1;
	m = (x%8) ? x/8 + 1 : x/8;
	
	p = new char[m];
	fill_n(p, m, 0xFF);
	if (x%8) p[m-1] ^= (0xFF >> x%8);
	
	clock_t t0 = clock();
	
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
	
	A->time = (clock() - (float) t0) / CLOCKS_PER_SEC;
	
	for (i = 0; i < m; ++i) {
		if (!(p[i])) continue;
		for (j = 0; j < 8; ++j) {
			if (!((p[i] >> (7-j)) & 1)) continue;
			x = c + 8*i + j;
			fprintf(A->file, "%d ", x);
			++sum;
		}
	}
	A->n = sum;
	
	delete[] p;
	return NULL;
	
}

int main(int argc, char *argv[]) {
	
	if (argc < 5) {
		cout << "./main N a b file" << endl;
		return 0;
	}
	bool flag = false;
	if (argc > 5) if ((argv[5][0] == '-') and (argv[5][1] == 'n')) flag = true;
	
	int size = atoi(argv[1]), a = atoi(argv[2]), b = atoi(argv[3]);
	if (size < 1) {
		cout << "N >= 1" << endl;
		return 0;
	}
	if (a < 2) a = 2;
	if ((b < a) or (b < 2)) {
		cout << "Неверный диапазон" << endl;
		return 0;
	}
	
	int sb = (int) trunc(sqrt(b));
	int x, n, i, j, k ,l, sum = 0;
	
	if (size == 1) x = b - 1;
		else x = sb-1;
	n = (x%8) ? x/8 + 1 : x/8;
	
	char *s = new char[n];
	fill_n(s, n, 0xFF);
	if (x%8) s[n-1] ^= (0xFF >> x%8);
	
	clock_t t0 = clock();
	
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
	
	float time = (clock() - (float) t0) / CLOCKS_PER_SEC;
	
	FILE *file = fopen(argv[4], "w");
	if (file == NULL) {
		cout << "Error opening file " << argv[4] << endl;
		return 1;
	}
	
	arg A[size-1];
	
	if (size > 1) {
		A[0].c = (a < sb+1) ? sb+1 : a;
		A[0].n = n;
		A[0].s = s;
		A[0].file = file;
		x = (b - A[0].c + 1) / (size-1);
		
		for (i = 1; i < size-1; ++i) {
			A[i].c = A[i-1].c + x;
			A[i-1].d = A[i].c - 1;
			A[i].n = n;
			A[i].s = s;
			A[i].file = file;
		}
		A[size-2].d = b;
	}
	
	pthread_t threads[size-1];
//	pthread_mutex_init(&lock, NULL);
	
	for (long id = 0; id < size - 1; id++)
		pthread_create(threads + id, NULL, work, A + id);
	
	if ((a < sb+1) or (size == 1)) {
		i = (a-2)/8;
		for (j = (a-2)%8; j < 8; ++j) {
			if (!((s[i] >> (7-j)) & 1)) continue;
			x = 8*i + j + 2;
			fprintf(file, "%d ", x);
			++sum;
		}
		for (++i; i < n; ++i) {
			if (!(s[i])) continue;
			for (j = 0; j < 8; ++j) {
				if (!((s[i] >> (7-j)) & 1)) continue;
				x = 8*i + j + 2;
				fprintf(file, "%d ", x);
				++sum;
			}
		}
	}
	
	float timeSUM = time, timeMAX = time;
	for(i = 0; i < size-1; i++) {
		pthread_join(threads[i], NULL);
		timeSUM += A[i].time;
		if (A[i].time > timeMAX) timeMAX = A[i].time;
		sum += A[i].n;
	}
	
	if (flag) cout << size << "\t" << timeSUM << "\t" << timeSUM / size << "\t" << timeMAX << endl;
		else {
		cout << "Найдено " << sum << " простых чисел" << endl;
		cout << "Потоков выполнения: " << size << endl;
		cout << "Суммарное время: " << timeSUM << endl;
		cout << "Среднее время: " << timeSUM / size << endl;
		cout << "Максимальное время: " << timeMAX << endl;
	}
	
	delete[] s;
	fclose(file);
	
	return 0;
	
}
