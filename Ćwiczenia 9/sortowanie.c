#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TEST         0

// Quick sort
void quick_sort (int *A, int len)
{
    if (len > 1) {
            int piv = *(A + len - 1);
        int a = 0, b = 0;
        int *B = A;

        for (int i=0; i<len-1; i++) {
            if ( *(A+i) < piv ) {
                
                // swap
                int buf = *(A+i);
                *(A+i) = *B;
                *B = buf;
                
                B++;
                a++;
            }
            else
                b++;
        }

        //swap
        *(A + len - 1) = *B;
        *B = piv;
        B++;

        quick_sort (A, a);
        quick_sort (B, b);
    }
}

int compare(const void * f, const void * s) {
    char *a = *(char**) f;
    char *b = *(char**) s;

    while (*a != '\0' || *b != '\0') {
        if (*a < *b)
            return -1;
        if (*a > *b)
            return 1;
        a++;
        b++;
    }

    return 0;
}


// Sortowanie słów
void sort_words(char *words[], int count)
{
    qsort(words, count, sizeof(char**), compare);
}

// Sortowanie leksykograficzne struktur

typedef struct structure
{
    char *name;
    char *nazwa;
} structure;

structure s[10];

//...............

int lexi_cmp(const void * f, const void * s) {
    /*
    char *a = *(char**) f.name;
    char *b = *(char**) s.name; */

    structure * a = (structure*) f;
    structure * b = (structure*) s;

    return  strcmp ( a->name, b->name );
    // return compare( a->name, b->name);
}

void struct_lexi_sort(int *len)
{
    qsort(s, *len, sizeof(structure), lexi_cmp);
}

// Złożone wymagania dotyczące sortowania

typedef struct struktura
{
    char * sname;
    char * fname;
    int age;
} struktura;

struktura sa[10];

//...............

int is_woman(struktura osoba) {
    char *name = osoba.sname;

    while ( *(name+1) != '\0' )
        name++;
    if ( *name == 'a' ) {
        //printf("%s is woman \n", osoba.sname);
        return 1;
    }
    return 0;
}

int person_cmp(const void * f, const void * s) {
    struktura * a = (struktura*) f;
    struktura * b = (struktura*) s;

    if (is_woman(*a) && !is_woman(*b))
        return -1;
    if (!is_woman(*a) && is_woman(*b))
        return 1;

    // Both woman
    if (is_woman(*a))
        return a->age - b->age;

    return  strcmp ( a->fname, b->fname );
}

void struct_name_sort(int *len)
{
    qsort(sa, *len, sizeof(struktura), person_cmp);
}

///////////////////////////////////////////////////


int main(void)
{
    int n=10;
    int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    char *words[] = { "dog", "ant", "cat", "man", "bear", "frog", "leopard", "bat", "rabbit", "monkey"};
    int to_do;

    if(TEST) printf("Wpisz nr zadania ");
    scanf ("%d", &to_do);

    switch (to_do)
    {
    case 1: // Quick sort
        quick_sort(a, n);
        for(int i = 0; i<n; i++)
        {
            printf("%d ", a[i]);
        }
        break;
    case 2:  // Sortowanie słów

        sort_words(words,n);

        for (int i = 0; i < n; i++)
        {
            printf("%s ", words[i]);
        }
        if(TEST) printf ("\n");
        break;
    case 3:  // Sortowanie leksykograficzne struktur
        s[0].name = "dog";
        s[0].nazwa = "pies";
        s[1].name = "ant";
        s[1].nazwa = "mrowka";
        s[2].name = "cat";
        s[2].nazwa = "kot";
        s[3].name = "Man";
        s[3].nazwa = "czlowiek";
        s[4].name = "bear";
        s[4].nazwa = "niedzwiedz";
        s[5].name = "frog";
        s[5].nazwa = "zaba";
        s[6].name = "leopard";
        s[6].nazwa = "lampart";
        s[7].name = "BAT";
        s[7].nazwa = "nietoperz";
        s[8].name = "rabbit";
        s[8].nazwa = "krolik";
        s[9].name = "monkey";
        s[9].nazwa = "malpa";

        struct_lexi_sort(&n);
        for (int i = 0; i < n; i++)
        {
            printf("%s ",s[i].name);
            if(TEST) printf("znaczy ");
            printf("%s ", s[i].nazwa);
            if(TEST) printf ("\n");
        } //ODKOMENTUJ W ZADANIU 3
        break;
    case 4:  // Złożone wymagania dotyczące sortowania
        n++;
        sa[0].sname = "Anastazja";
        sa[0].fname = "Kamiska";
        sa[0].age = 28;
        sa[1].sname = "Stefan";
        sa[1].fname = "Kowal";
        sa[1].age = 36;
        sa[2].sname = "Kamila";
        sa[2].fname = "Munk";
        sa[2].age = 25;
        sa[3].sname = "Ernest";
        sa[3].fname = "Goja";
        sa[3].age = 27;
        sa[4].sname = "Karol";
        sa[4].fname = "Wodecki";
        sa[4].age = 27;
        sa[5].sname = "Anna";
        sa[5].fname = "Stone";
        sa[5].age = 15;
        sa[6].sname = "Katarzyna";
        sa[6].fname = "Michalak";
        sa[6].age = 20;
        sa[7].sname = "Małgorzata";
        sa[7].fname = "Kowal";
        sa[7].age = 33;
        sa[8].sname = "Emil";
        sa[8].fname = "Kant";
        sa[8].age = 22;
        sa[9].sname = "Kamil";
        sa[9].fname = "Mist";
        sa[9].age = 29;
        sa[10].sname = "Tomasz";
        sa[10].fname = "Nod";
        sa[10].age = 18;
        struct_name_sort(&n);
        for (int i = 0; i < n; i++)
        {
            printf("%s %s %d ",sa[i].sname, sa[i].fname, sa[i].age);
            if(TEST) printf ("\n");
        } //ODKOMENTUJ W ZADANIU 4
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
