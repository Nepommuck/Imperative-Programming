// UWAGA
// 
// Zadanie robione na "odwal sie", radze nie kopiowac


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

double get(const int l_kol, const int x, const int y, double tab[]) {
    return tab[y * l_kol + x];
}

void set(const int, const int, const int, double*, const double);

void set(const int l_kol, const int x, const int y, double tab[], const double new_val) {
    tab[y * l_kol + x] = new_val;
}

void prod_mat(const int, const int, const int, const double*, const double*, double*);

// prod_mat(rowsA, colsA, colsB, A, B, C)
// C: (rowsA x colsB)
void prod_mat(const int rowsA, const int colsA, const int colsB, const double a[], const double b[], double rez[]) {
    for (int y=0; y<rowsA; y++) {
        for (int x=0; x<colsB; x++) {
            double elem = 0;
            for (int i=0; i<colsA; i++)
                elem += get(colsA, i, y, a) * get(colsB, x, i, b);
            set(colsB, x, y, rez, elem);
        }
    }
}

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
int read_char_lines(char *tab[], int lines_count, FILE *stream) {
    int i=0;

    while(!feof(stream)) {
        char *ptr = malloc(100 * sizeof(char));
        tab[i] = fgets(ptr, 100, stream);
        i++;
    } /*
    for (int k=0; k<i; k++) {
        printf("%d: %s", k, tab[k]);
    } */
}


void write_char_line(char *tab[], int n) {
    printf("%s", tab[n]);
}

// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_lines(char *tab[], int line_count) {
    // to do later
}


// 8.1.2.  Funkcje dla tablicy wierszy liczbowych w obszarze ciągłym
// Wskazane uzycie funkcji fgets(), strtod().

int read_dbl_lines_v1(double *ptr_tab[], int lines_counter, FILE *stream) {
/*
    char *slow;
    double liczby[20][100];
    while(!feof(stream)) {
        fgets(slow, 100, stream);
        int i=0;
        /*
        while(*slow != '\0') {
            char **next;
            liczby[lines_counter][i] = strtod(slow, next);
            slow = *next;
        }
        lines_counter++;
    }
*/
    return 1;
}

void write_dbl_line_v1(double *ptr_tab[], int n, FILE *stream) {
    char slow[100];

    //for (int i=0; i<=n; i++) {
    int i = 1;
    while(!feof(stream)) {
        fgets(slow, 100, stream);
        if (i == n)
            printf("%s", slow);
        i++;
    }
}


// 8.1.4.  Funkcje dla tablicy wierszy liczbowych w odrebnych obszarach
// Wskazane uzycie funkcji fgets(), strtod(), malloc().

struct line {
    double *values;
    int len;
    double average;
};

typedef struct line line_type;


int read_dbl_lines(line_type lines_tab[], int line_count, FILE *stream) {
    char *zdanie = (char*) malloc(BUF_SIZE * sizeof(char));

    int i=0;
    fgets(zdanie, BUF_SIZE, stream);
    while (!feof(stream)) {
        //printf("%s \n", zdanie);

        char *ob = zdanie;
        char *last = ob;
        double *nums = (double*) malloc(TAB_SIZE * sizeof(double));
        int n=0;

        while (*last != '\0' && *last != '\n') {
            nums[n] = strtod(ob, &last);
            //printf("Wyluskano: %f \n", nums[n]);
            ob = last;
            n++;
        }
        double sum = 0;
        lines_tab[i].len = n;
        lines_tab[i].values = (double*) malloc(n * sizeof(double));
        for (int k=0; k<n; k++) {
            lines_tab[i].values[k] = nums[k];
            sum += nums[k];
        }
        lines_tab[i].average = sum / (double) n;

        fgets(zdanie, BUF_SIZE, stream);
        i++;
    }
    return i;
}

int cmp (const void *a, const void *b) {
    line_type *fir = (line_type*) a;
    line_type *sec = (line_type*) b;

    return (*fir).average - (*sec).average;
}

void sort_by_average(line_type lines_tab[], int line_count) {
    qsort(lines_tab, line_count, sizeof(line_type), cmp);
}

void write_dbl_line(line_type lines_tab[], int n){
    int k = lines_tab[n].len;
    for (int i=0; i<k; i++)
        printf("%f ", lines_tab[n].values[i]);
    printf("\n");
}

void print_lines(line_type lines_tab[], int line_count) {

    for (int i=0; i<line_count; i++) {
        printf("-------------- \n");
        printf("Line %d: \n", i);
        printf("len =  %d \n", lines_tab[i].len);
        printf("avg =  %f \n", lines_tab[i].average);
        printf("Nums: [");
        for (int k=0; k<lines_tab[i].len; k++) {
            printf("%f, ", lines_tab[i].values[k]);
        }
        printf("] \n\n\n");
    }
}

// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_dbl_lines(line_type *tab[], int line_count) {
    free(tab);
}



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
        // lines_counter = read_dbl_lines_v1(ptr_table,0,stream);
        write_dbl_line_v1(ptr_table,n, stream);
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
        //delete_lines(char_lines_table,lines_counter);
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
        print_lines(dbl_lines_table, lines_counter);
        write_dbl_line(dbl_lines_table, n-1);
        delete_dbl_lines(dbl_lines_table, lines_counter);

        break;
default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
  return 0;
}
