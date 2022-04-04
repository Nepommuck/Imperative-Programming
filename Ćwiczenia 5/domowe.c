#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 1     // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], int n) {
  for(int i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(double x[], int n) {
  for(int i=0; i<n; ++i) printf("%.4f ",x[i]);
  printf("\n");
}

void print_int_vec(int x[], int n) {
    printf("[ ");
    for(int i=0; i<n; ++i)
        printf("%d ",x[i]);
  printf("] \n");
}

void read_mat(double A[][SIZE], int m, int n) {
  for(int i=0; i<m; ++i) {
    for(int j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
  }
}

void print_mat(double A[][SIZE], int m, int n) {
  for(int i=0; i<m; ++i) {
    for(int j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
    printf("\n");
  }
}

void print_mat_ind(double A[][SIZE], int m, int n,int indicies[]) {
  for(int i=0; i<m; ++i) {
    for(int j=0; j<n; ++j)  printf("%.4f ",A[indicies[i]][j]);
    printf("\n");
  }
  printf("\n");
}

// A mxp, B pxn
// definicja powinna byc juz napisana w zadaniu 5.1.1
void mat_product(const double A[][SIZE], const double B[][SIZE], double AB[][SIZE], const int m, const int p, const int n) {
    for (int y=0; y<m; y++) {
        for (int x=0; x<n; x++) {
            double rez = 0;
            for (int iter=0; iter<p; iter++)
                rez += A[y][iter] * B[iter][x];
            AB[y][x] = rez;
        }
    }
}

// Funkcja moze byc przydatna do sprawdzania poprawnosci rozwiazania Ax=b
void mat_vec_product(double A[][SIZE], double b[], double Ab[], int m, int n) {
  int i, k;
  double sum;
  for(i=0; i<m; ++i) {
    for(k=0, sum=0.0; k<n; ++k) sum += A[i][k] * b[k];
    Ab[i] = sum;
  }
  return;
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    double det = 1.0;
    int p[n];
    double b_cop[n];
    for (int i=0; i<n; i++) {
        p[i] = i;
        b_cop[i] = b[i];
    }

    for (int d=0; d<n; d++) {

        int mx = d;
        for (int y=d+1; y<n; y++)
            if (fabs(A[ p[y] ][d]) > fabs(A[ p[mx] ][d]))
                mx = y;
        if (mx != d) {
            // swap p[d] <-> p[mx]
            int tmp = p[d];
            p[d] = p[mx];
            p[mx] = tmp;

            det *= (-1.0);
        }

        // Niedokonczone
        if ( fabs(A[ p[d] ][d]) < eps)
            return 0.0;

        for (int y=d+1; y<n; y++)
            // if A[y][d] != 0
            if ( fabs(A[ p[y] ][d]) > eps) {

                double q = -A[ p[y] ][d] / A[ p[d] ][d];
                for (int x=d; x<n; x++)
                    A[ p[y] ][x] += q * A[ p[d] ][x];
                b_cop[ p[y] ] += q * b_cop[ p[d] ];
            }
    }

    for (int d=0; d<n; d++)
        det *= A[ p[d] ][d];

    for (int y=n-1; y>=0; y--) {
        double rez = 0;
        for (int i=n-1; i>y; i--) {
            rez += A[ p[y] ][i] * x[i];
        }

        x[y] = (b_cop[ p[y] ] - rez) / A[ p[y] ][y];
    }
    return det;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], const int n, const double eps) {
    double det = 1.0;
    int p[n];
    double B_cop[n][n];

    for (int i=0; i<n; i++)
        p[i] = i;
    for (int y=0; y<n; y++)
        for (int x=0; x<n; x++)
            B_cop[y][x] = 0.0;
    for (int d=0; d<n; d++)
        B_cop[d][d] = 1.0;

    // ETAP I
    for (int d=0; d<n; d++) {

        int mx = d;
        for (int y=d+1; y<n; y++)
            if (fabs(A[ p[y] ][d]) > fabs(A[ p[mx] ][d]))
                mx = y;
        if (mx != d) {
            // swap p[d] <-> p[mx]
            int tmp = p[d];
            p[d] = p[mx];
            p[mx] = tmp;

            det *= (-1.0);
        }

        double q = A[ p[d] ][d];
        det *= q;

        // Niedokonczone
        if ( fabs(q) < eps )
            return 0.0;

        for (int x=0; x<n; x++) {
            A[ p[d] ][x] /= q;
            B_cop[ p[d] ][x] /= q;
        }


        for (int y=d+1; y<n; y++) {
            q = A[ p[y] ][d];
            for (int x=0; x<n; x++) {
                A[ p[y] ][x] -= q * A[ p[d] ][x];
                B_cop[ p[y] ][x] -= q * B_cop[ p[d] ][x];
            }
        }
        /*
        printf("A: \n");
        print_mat_ind(A, n, n, p);
        printf("B: \n");
        print_mat_ind(B_cop, n, n, p);  */
    }

    // ETAP II
    for (int d=n-1; d>=0; d--) {
        for (int y=d-1; y>=0; y--) {
            double q = A[ p[y] ][d];
            for (int x=0; x<n; x++) {
                A[ p[y] ][x] -= q * A[ p[d] ][x];
                B_cop[ p[y] ][x] -= q * B_cop[ p[d] ][x];
            }
        }   /*
        printf("A: \n");
        print_mat_ind(A, n, n, p);
        printf("B: \n");
        print_mat_ind(B_cop, n, n, p);  */
    }

    for (int i=0; i<n; i++)
        for (int x=0; x<n; x++)
            B[i][x] = B_cop[ p[i] ][x];

    return det;
}

int main(void) {

  double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
  double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m,n,p,q;
	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);

	switch (to_do) {
    case 1:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      if(TEST) printf("Wpisz wektor b: ");
      read_vec(b,n);
      det = gauss(A, b, x, n, eps);
      printf("%.4f\n",det);
      if(det) print_vec(x,n);
      break;
    case 2:
      if(TEST) printf("Wpisz rozmiar macierzy n = ");
      scanf("%d",&n);
      if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
      read_mat(A,n,n);
      det = matrix_inv(A,B,n,eps);
      printf("%.4f\n",det);
      if(det) print_mat(B,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
