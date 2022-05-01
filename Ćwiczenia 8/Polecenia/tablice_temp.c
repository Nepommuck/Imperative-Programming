#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000
#define TEST         1

// Funkcje dla punktu 8.1.1 Macierze prostokatne

void copy_mat(const int, int const, double*, const double*);

//  Oblicza S = A + B
void sum_mat(int, int, const double*, double*, double*);

double get(const int, const int, const int, double*);

void set(const int, const int, const int, double*, const double);

void prod_mat(const int, const int, const int, const double*, const double*, double*);

void prod_mat_v2(const int, const int, const int, ...   );
// Nalezy uzupelnic liste parametrow - z parametrami typu tablicy 1D
// W ciele funkcji nalezy zdefiniowaæ wskazniki do tablic 2D, do ktorych nalezy przepisac
// (z odpowiednim rzutowaniem) parametry A, B i AB.
// W algorytmie mnozenia macierzy bedzie mozna stosowac notacje indeksowa X[i][j]

void prod_mat_v3(const int, const int, const int, ...   );
// Nalezy uzupelnic liste parametrow - z parametrami typu tablicy 2D
// W algorytmie mnozenia macierzy bedzie mozna stosowac notacje indeksowa X[i][j]

void read_mat(const int rows, const int cols, double *t) {
    for(int i=0; i<rows*cols; ++i, ++t) scanf("%lf",t);
}

void print_mat(int rows, int cols, double *t) {
  for(int  r=0; r<rows; ++r) {
    for(int c=0; c<cols; ++c) printf("%.2f ",*t++);
    printf("\n");
  }
}


// Funkcje do punktow 8.1.2, 8.1.3 i 8.1.4


// 8.1.3.  Funkcje dla tablicy wierszy znakowych
// Wskazane uzycie funkcji fgets(), memchr(), malloc()
int read_char_lines(char *tab[], int lines_count, FILE *stream);

void write_char_line(char *tab[], int n) {

// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_lines(char *tab[], int line_count);


// 8.1.2.  Funkcje dla tablicy wierszy liczbowych w obszarze ciągłym
// Wskazane uzycie funkcji fgets(), strtod().

int read_dbl_lines_v1(double *ptr_tab[], int lines_counter, FILE *stream);

void write_dbl_line_v1(double *ptr_tab[], int n);


// 8.1.4.  Funkcje dla tablicy wierszy liczbowych w odrebnych obszarach
// Wskazane uzycie funkcji fgets(), strtod(), malloc().

struct line {
    double *values;
    int len;
    double average;
};

typedef struct line line_type;

int read_dbl_lines(line_type lines_tab[], int line_count, FILE *stream);

void write_dbl_line(line_type lines_tab[], int n);

int cmp (const void *a, const void *b);

// Nalezy zastosowac funkcje qsort() z wywolaniem funkcji cmp().
void sort_by_average(line_type lines_tab[], int line_count);

// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_dbl_lines(line_type *tab[], int line_count);



/////////////////////////////////////////////////////////////////////////

int main(void) {
    FILE *stream;
    char file_name[50];
    int to_do;
   	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	stream = stdin;

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    int n, lines_counter, rowsA,colsA,rowsB,colsB;
    char *char_lines_table[TAB_SIZE];
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];
    line_type dbl_lines_table[TAB_SIZE];

    switch (to_do) {
case 1: // Macierze prostkatne
        if(TEST) printf("Wpisz liczbe wierszy i kolumn oraz elementy 1. macierzy ");
        scanf("%d %d",&rowsA,&colsA);
        read_mat(rowsA, colsA, A);
        if(TEST) printf("Wpisz liczbe wierszy i kolumn oraz elementy 2. macierzy ");
        scanf("%d %d",&rowsB,&colsB);
        read_mat(rowsB, colsB, B);
        prod_mat(rowsA,colsA,colsB,A,B,C);
        print_mat(rowsA,colsB,C);
        break;
case 2:  // Ciagi liczbowe w ciaglym obszarze, wiersze roznej dlugosci:
         // ptr_table[lines_counter] points out 1. byte of free memory
        ptr_table[0] = series_table;
        if(TEST) printf("Wpisz numer wiersza (liczony od 1) ");
        scanf("%d",&n);
        if(TEST) printf("Wpisz kolejne linie zawierajace liczby, zakoncz znakiem EOF\n");
        if(TEST == 0) {
            scanf("%s",file_name);
            stream = fopen(file_name,"r");
            if(stream == NULL) { printf("fopen failed\n");  return -1; }
        }
        lines_counter = read_dbl_lines_v1(ptr_table,0,stream);
        write_dbl_line_v1(ptr_table,n);
        break;
case 3:  // Linie znakow w odrebnych obszarach pamieci:
        if(TEST) printf("Wpisz numer wiersza (liczony od 1) ");
        scanf("%d",&n);
        if(TEST) printf("Wpisz kolejne linie zawierajace znaki ASCII, zakoncz znakiem EOF\n");
        if(TEST == 0) {
            scanf("%s",file_name);
            stream = fopen(file_name,"r");
            if(stream == NULL) { printf("fopen failed\n");  return -1; }
        }
        lines_counter = read_char_lines(char_lines_table, 0, stream);
        write_char_line(char_lines_table,n-1);
        delete_lines(char_lines_table,lines_counter);
        break;
case 4:  // Ciagi liczbowe w odrebnych obszarach pamieci + sortowanie:
        if(TEST) printf("Wpisz numer wiersza (liczony od 1) w kolejnosci rosnacej sredniej ");
        scanf("%d",&n);
        if(TEST) printf("Wpisz kolejne linie zawierajace liczby, zakoncz znakiem EOF\n");
        if(TEST == 0) {
            scanf("%s",file_name);
            stream = fopen(file_name,"r");
            if(stream == NULL) { printf("fopen failed\n");  return -1; }
        }
        lines_counter = read_dbl_lines(dbl_lines_table, 0, stream);
        sort_by_average(dbl_lines_table, lines_counter);
        write_dbl_line(dbl_lines_table, n-1);
        delete_dbl_lines(dbl_lines_table, lines_counter);
        break;
default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
  return 0;
}
