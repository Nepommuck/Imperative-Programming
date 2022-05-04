#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAB_SIZE  1000
#define SUCCESS   1
#define ERROR     0
#define TEST      0

enum matrix {
    square,
    symmetric,
    lower_triangular,
    upper_triangular
};

typedef enum matrix mat_types;


// Parametr size jest liczba wierszy (oraz kolumn) macierzy
double s_get(int size, int row, int col, double *tab, mat_types mt) {
    if (mt == square)
        return tab[size*row + col];
    if (mt == symmetric || mt == lower_triangular) {
        if (col > row)
            return 0.0;
        return tab[row * (row+1) / 2 + col];
    }
    if (mt == upper_triangular) {
        if (col < row)
            return 0.0;
        return tab[row * size - row * (row+1) / 2 + col];
    }
}


// Zwraca: 1 - sukces, 0 - blad
int s_set(int size, int row, int col, double *tab, mat_types mt, double value) {
    if (row < 0 || row >= size || col < 0 || col >= size)
        return 0;

    else if (mt == square)
        tab[size*row + col] = value;

    else if (mt == symmetric || mt == lower_triangular) {
        if (col > row)
            return 0;
        tab[row * (row+1) / 2 + col] = value;
    }
    else if (mt == upper_triangular) {
        if (col < row)
            return 0;
        tab[row * size - row * (row+1) / 2 + col] = value;
    }
    return 1;
}


int s_mat_copy(int size, double *B, mat_types mtB, double *A, mat_types mtA) {
    for (int x=0; x<size; x++)
        for (int y=0; y<size; y++)
            s_set(size, y, x, B, mtB, s_get(size, y, x, A, mtA));
}


// Elementy kolejnych kolumn kolejnych wierszy macierzy (dowolnego typu)
// sa wpisywane do kolejnych elementow tablicy jednowymiarowej.
void s_mat_read(int size, double *A, mat_types mt) {
    int elems;

    if (mt == square)
        elems = size * size;

    else if (mt == symmetric || mt == lower_triangular || mt == upper_triangular)
        elems = size * (size+1) / 2;

    for (int i=0; i<elems; i++)
        scanf("%lf", &(A[i]));
}


void s_mat_print(int size, double *M, mat_types mt) {
    for (int i=0; i<size; i++) {
        for (int k=0; k<size; k++)
            printf("%.2f ", s_get(size, i, k, M, mt));
        printf("\n");
    }
}


void s_mat_prod(int size, double A[], mat_types mtA, double B[], mat_types mtB, double AB[], mat_types mtAB) {
    /*
    for (int i=0; i<size; i++)
        for (int k=0; k<size; k++)
            s_set(size, i, k, AB, mt, 0.0); */

    for (int y=0; y<size; y++)
        for (int x=0; x<size; x++) {
            double rez = 0;
            for (int k=0; k<size; k++)
                rez += s_get(size, y, k, A, mtA) * s_get(size, k, x, B, mtB);
            // printf("%f \n", rez);
            s_set(size, y, x, AB, mtAB, rez);
        }
}


int r,c,k; // ???

// B := inv(A)
int s_mat_inv(const int size, double B[], mat_types mtB, double A[], mat_types mtA) {
    for (int i=0; i<size; i++)
        s_set(size, i, i, B, mtB, 1 / s_get(size, i, i, A, mtA));

    for (int x=0; x<size; x++) {
        for (int y=x+1; y<size; y++) {
            double sum = 0;
            for (int q=x; q<= y-1; q++)
                sum += s_get(size, y, q, A, mtA) * s_get(size, q, x, B, mtB);
            s_set(size, y, x, B, mtB, -s_get(size, y, y, B, mtB) * sum);
        }
    }
}


int s_mat_inv_in_situ(const int size, double A[], mat_types mtA) {
    for (int i=0; i<size; i++)
        s_set(size, i, i, A, mtA, 1 / s_get(size, i, i, A, mtA));

    for (int x=0; x<size; x++) {
        for (int y=x+1; y<size; y++) {
            double sum = 0;
            for (int q=x; q<= y-1; q++)
                sum += s_get(size, y, q, A, mtA) * s_get(size, q, x, A, mtA);
            s_set(size, y, x, A, mtA, -s_get(size, y, y, A, mtA) * sum);
        }
    }
}


/////////////////////////////////////////////////////////////


int main(void) {
    // mat_types m = square;
    // printf("%d \n", m == square);

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

        //s_mat_print(size, A, mtA);

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
