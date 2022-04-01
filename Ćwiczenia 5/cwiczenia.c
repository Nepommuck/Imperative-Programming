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

// 5.1.0
#define STRLEN  10
#define KW_NO   32    // liczba zapisanych lancuchow (slow kluczowych)

// Kopiuje lancuchy wskazywane w tablicy wskaznikow tw do tablicy t2D
void str_copy(char t2D[][STRLEN], char *tw[], const int n) {
    for (int i=0; i<n; i++) {
        strcpy(t2D[i], tw[i]);
    }
}

// Sortuje alfabetycnie tablice wskaznikow t
void sortuj_tw(char *t[], const int n) {
    /*
    for (int i=0; i<n; i++)
        printf("%s \n", t[i]);  */

    for (int i=n-1; i>=0; i--) {
        for (int k=0; k<i; k++) {
            if (strcmp(t[k], t[k+1]) > 0) {
                char *tmp = t[k];
                t[k] = t[k+1];
                t[k+1] = tmp;
            }
        }
    }
}

// Porzadek alfabetyczny lancuchow zapisanych w tablicy t2D zapisuje w tablicy indx
// Uwaga: Tablica t2D jest typu const char
void sortuj_t2D(const char t2D[][STRLEN], int indx[], const int n) {
    for (int i=0; i<n; i++)
        indx[i] = i;

    for (int i=n-1; i>=0; i--) {
        for (int k=0; k<i; k++) {
            if (strcmp(t2D[ indx[k] ], t2D[ indx[k+1] ]) > 0) {
                int tmp = indx[k];
                indx[k] = indx[k+1];
                indx[k+1] = tmp;
            }
        }
    }
}

// W wierszach tablicy t2D sa zapisane lancuchy znakowe w dowolnej kolejnosci.
// Tablica indeksow wierszy indx wyznacza alfabetyczny porzadek tych lancuchow.
// Funkcja wypisuje w osobnych liniach n poczatkowych (wg alfabetu) lancuchow.
void print_t2D(const char t2D[][STRLEN], const int indx[], const int n) {
    for (int i=0; i<n; i++)
        printf("%s \n", t2D[ indx[i] ]);
}

// 5.1.1
// Funkcja wstawia do macierzy AB iloczyn macierzy A mxp i B pxn (w tej kolejnosci czynnikow)
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

// 5.1.2 Triangulatyzacja macierzy i obliczanie wyznacznika - wersja uproszczona
// (bez zamiany wierszy).
// Jezeli element glowny a[i][i] = 0, to triangularyzacja nie jest dokonczona,
// a wyznacznik = NAN
// Zalozenie: funkcja gauss moze zmienic wartosci elementow tablicy A
double gauss_simplified(double A[][SIZE], const int n) {
    double w = 1;
    for (int x=0; x<n; x++) {
        double dziel = A[x][x];
        // 0 na przekatnej
        if (dziel == 0)
            return NAN;
        w *= dziel;
        for (int k=0; k<n; k++)
            A[x][k] /= dziel;
        for (int y=x+1; y<n; y++) {
            dziel = A[y][x];
            if (dziel != 0) {
                w *= dziel;
                for (int k=x; k<n; k++) {
                    A[y][k] /= dziel;
                    A[y][k] -= A[x][k];
                }
            }
        }
    }

    return w;
}

int main(void) {

  double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
  double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m,n,p,q;

	char *keywords_tw[] = {"struct", "typedef", "for", "union",
    "int", "void", "long", "register", "auto", "return",
    "double", "else", "sizeof", "enum", "const", "continue",
    "default", "do", "short", "extern", "static", "float",
    "goto", "switch", "if", "unsigned", "volatile",
    "while", "signed", "break", "char", "case"};
  char keywords_t2D[KW_NO][STRLEN];
  int indx[KW_NO];

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
    case 0:
      str_copy(keywords_t2D, keywords_tw, KW_NO);
      sortuj_tw(keywords_tw,KW_NO);
      sortuj_t2D(keywords_t2D,indx,KW_NO);
      if(TEST) printf("Wpisz liczbe slow ");
      scanf("%d", &n);
      print_t2D(keywords_t2D, indx, n);
      break;
    case 1:
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn mac.1: ");
      scanf("%d %d", &m, &n);
      if(TEST) printf("Wpisz mac.1 (wierszami): ");
      read_mat(A,m,n);
      if(TEST) printf("Wpisz liczbe wierszy i liczbe kolumn mac.2: ");
      scanf("%d %d", &p, &q);
      if(TEST) printf("Wpisz mac.2 (wierszami): ");
      read_mat(B,p,q);
      if(n == p) mat_product(A,B,C,m,n,q);
      print_mat(C,m,q);
      break;
    case 2:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      printf("%.4f", gauss_simplified(A, n));
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}
