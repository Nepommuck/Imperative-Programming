#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAB_SIZE 1000
#define SUCCESS  1
#define ERROR    0
#define TEST         1

// Nalezy uzupelnic definicje typu
typedef  .....  mat_types;

// Parametr size jest liczba wierszy (oraz kolumn) macierzy

double s_get(int size, int row, int col, double *tab, mat_types mt);
// Zwraca: 1 - sukces, 0 - blad
int s_set(int size, int row, int col, double *tab, mat_types mt, double value);

int s_mat_copy(int size, double *B, mat_types mtB, double *A, mat_types mtA);

// Elementy kolejnych kolumn kolejnych wierszy macierzy (dowolnego typu)
// sa wpisywane do kolejnych elementow tablicy jednowymiarowej.
void s_mat_read(int size, double *A, mat_types mt);

void s_mat_print(int size, double *M, mat_types mt);

void s_mat_prod(int size, double A[], mat_types mtA, double B[], mat_types mtB, double AB[], mat_types mtAB);  int r,c,k;

// B := inv(A)
int s_mat_inv(const int size, double B[], mat_types mtB, double A[], mat_types mtA);

int s_mat_inv_in_situ(const int size, double A[], mat_types mtA);

/////////////////////////////////////////////////////////////

int main(void) {
    int to_do;
   	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);

    int size;
    double A[TAB_SIZE],B[TAB_SIZE],C[TAB_SIZE];
    mat_types mtA,mtB,mtC;

    switch (to_do) {
case 1: // Mnozenie macierzy
        if(TEST) printf("Wpisz rozmiar (liczbe wierszy) macierzy ");
        scanf("%d",&size);
        if(TEST) printf("Wpisz typ 1. macierzy (0-sqr., 1-symetr., 2-LT, 3-UT) ");
        scanf("%d",&mtA);
        if(TEST) printf("Wpisz elementy 1. macierzy (wierszami)\n");
        s_mat_read(size, A, mtA);
        if(TEST) printf("Wpisz typ 2. macierzy (0-sqr., 1-symetr., 2-LT, 3-UT) ");
        scanf("%d",&mtB);
        if(TEST) printf("Wpisz elementy 2. macierzy (wierszami)\n");
        s_mat_read(size, B, mtB);
        if(TEST) printf("Wpisz typ iloczynu macierzy (0-sqr., 1-symetr., 2-LT, 3-UT) ");
        scanf("%d",&mtC);

        s_mat_prod(size, A, mtA, B, mtB, C, mtC);
        s_mat_print(size, C, mtC);
        break;
case 2:  // Odwracanie macierzy trojkatnej dolnej
        if(TEST) printf("Wpisz rozmiar (liczbe wierszy) macierzy trojkatnej dolnej ");
        scanf("%d",&size);
        if(TEST) printf("Wpisz elementy macierzy (wierszami)\n");
        s_mat_read(size,A,lower_triangular);
        s_mat_inv(size,B,lower_triangular,A,lower_triangular);
        s_mat_print(size,B,lower_triangular);
        s_mat_prod(size,B,lower_triangular,A,lower_triangular,C,lower_triangular);
        s_mat_print(size,C,lower_triangular);
        break;
case 3:  // Odwracanie macierzy trojkatnej dolnej in_situ
        if(TEST) printf("Wpisz rozmiar (liczbe wierszy) macierzy trojkatnej dolnej ");
        scanf("%d",&size);
        if(TEST) printf("Wpisz elementy macierzy (wierszami)\n");
        s_mat_read(size,A,lower_triangular);
        s_mat_copy(size,B,lower_triangular,A,lower_triangular);
        s_mat_inv_in_situ(size,A,lower_triangular);
        s_mat_print(size,A,lower_triangular);
        s_mat_prod(size,A,lower_triangular,B,lower_triangular,C,lower_triangular);
        s_mat_print(size,C,lower_triangular);
        break;
default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
  return 0;
}
