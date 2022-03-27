#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TEST 1    // 1 - ulatwia testowanie programu,
// 0 - dla wersji przeznaczonej do automatycznej oceny

// Zwraca wartość średniej arytmetycznej oraz wariancji elemntów tablicy o długości n.
// dla n = 0 zwracana jest pusta tablica
// dla n = 1 zwracana jest tablica jednoelementowa
void aver_varian(double v[], double *arith_aver, double *varian, int n)
{
// ...
}

// Wypełnianie tablicy wynikiem n prób Bernoulliego
// 1 - orzeł; 0 - reszka
// Losowanie realizowane na 100-elemntowym zbiorze liczb całkowitych
void bernoulli_gen(int v[], int bernoulli_prop, int n)
{
// ...
}

// Dyskretny rozkład prawdopodobieństwa reprezentujący procentowy udział poszczególnych wartości zmiennej losowej
// liczba elementów biorących udział w losowaniu to 11 bo na 2 kostkach możemy uzyskać liczby [2-12]
// trials_num - liczba losowań

void probab_distrib(double v[], int trials_num)
{
// ...
}

// Oblicza i zwraca Dystrybuantę (Cumulative Distribution Function)
// len - liczba wylosowanych elementów z przediału
void var_dystryb(double dystr[], int len)
{
// ...
}

// Symuluje problem Monty'ego-Hall'a dla przypadku 3-bramokowego
//Wejście: n - liczba prób
//Wyjście: switch_wins - liczba wygranych przez zamianę, switch_probabil - prawdopodobieństwo wygranej przez zamianę
//Wyjście: non_switch_wins - liczba wygranych bez zamiany, non_switch_probabil - prawdopodobieństwo wygranej bez zamiany
void monty_hall(int *switch_wins, int *nonswitch_wins, double *switch_probabil, double *nonswitch_probabil, int n)
{
    int choice, door[3];
    for(int i=0; i<n; i++)
    {
        door[0] = (!(rand()%2)) ? 1: 0;                                         //przyjmij, że za drzwiami 1 jest nagroda lub zonk.
        // ...                                         //jeżeli za drzwiami 1 jest nagroda, przyjmij, że za pozostałymi jest zonk.
        else
        {
            door[1] = (!(rand()%2)) ? 1: 0;    //w przeciwnym wypadku, przyjmij, że za drzwiami 2 jest nagroda lub zonk, przyjmij, że za drzwiami 3 jest to co zostało.
            // ...
        }
        choice = rand()%3;                                                      //wybierz losowo drzwi.

        //jeśli za następnymi drzwiami jest zonk, a za kolejnymi nagroda lub odwrotnie, to wygrasz, jeśli je zmienisz.
        // ...
        //jeśli nie zmienisz swojego wyboru do końca
        // ...
    }
    // ... //prawdopodobieństwo wyrażone w procentach
    // ...
}

// read double vector of size n
void read_vector(double v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        scanf("%lf", v + i);
    }
}

// read int vector of size n
void read_int_vector(int v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", v + i);
    }
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector(const double v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%.2f ", v[i]);
    }
    printf("\n");
}

// print integer vector
void print_bern_vector(int bern_v[], int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf("%d ", bern_v[i]);
    }
    printf("\n");
}

int main(void)
{
    int to_do, len, seed, propabl, number, m_h_wins, non_m_h_wins, itab[100];
    double arith_aver, varian, m_h_probabil, non_m_h_probabil, dtab[100];
    if (TEST) printf("Wpisz numer testu [1, 5]: ");
    scanf("%d", &to_do);
    if(to_do>1)
    {
        if (TEST) printf("seed = ");
        scanf("%d",&seed);
        srand(seed);        // ustawienie ziarna dla powterzalnosci wynikow

    }

    switch (to_do)
    {
    case 1: // aver_varian
        arith_aver=0.0;
        varian=0.0;
        if (TEST) printf("aver_varian: Wpisz dlugosc wektora: ");
        scanf("%d", &len);
        if(len < 0 || len > 100)
        {
            printf("n<0 lub n > 100\n");
            break;
        }
        if (TEST) printf("Wpisz elementy wektora: ");
        read_vector(dtab, len);
        aver_varian(dtab, &arith_aver, &varian, len);
        if (TEST) printf("średnia arith_aver = ");
        printf("%.2f ", arith_aver);
        if (TEST) printf(" wariancja = ");
        printf("%.2f\n ", varian);
        break;
    case 2: // bernoulli_gen
        if (TEST) printf("BERNOULLI_GEN: Wpisz dlugość wektora: ");
        scanf("%d", &len);
        if(len < 0 || len > 100)
        {
            printf("n<0 lub n > 100\n");
            break;
        }
        if (TEST) printf("Wpisz prawdopodobieństwo reszki w %%: ");
        scanf("%d", &propabl);
        bernoulli_gen(itab, propabl, len);
        print_bern_vector(itab, len);
        break;
    case 3: // probab_distrib
        if (TEST) printf("PROBAB_DISTRIB: Wpisz liczbę prób: ");
        scanf("%d", &len);
        if(len < 0 || len > 1000000)
        {
            printf("n < 0 lub n > 1000000\n");
            break;
        }
        probab_distrib(dtab, len);
        print_vector(dtab, 11); //
        break;
    case 4: // var_dystryb
        if (TEST) printf("VAR_DYSTRYB: Wpisz liczbę rzutów dwiema kostkami: ");
        scanf("%d", &len);
        if(len < 0 || len > 1000000)
        {
            printf("n<0 lub n > 1000000\n");
            break;
        }
        if (TEST) printf("Wpisz elementy wektora: ");
        var_dystryb(dtab, len);
        print_vector(dtab, 11); //11 ponieważ w przypadku rzutu 2 kostkami istnieje 11 wartości zmiennej losowej [2-12]
        break;
    case 5: // monty_hall
        m_h_wins = non_m_h_wins=0;
        m_h_probabil = non_m_h_probabil = 0.0;
        if (TEST) printf("MONTY_HALL: Wpisz liczbę prób: ");
        scanf("%d", &len);
        if(len < 0 || len > 1000000)
        {
            printf("n<0 lub n > 1000000\n");
            break;
        }
        monty_hall(&m_h_wins, &non_m_h_wins, &m_h_probabil, &non_m_h_probabil, len);
        if (TEST) printf("Po %d grach, wygrałem ", len);
        printf("%d ", m_h_wins);
        if (TEST) printf("razy przez zmianę wyboru. To jest [%%] ");
        printf("%.2f ", m_h_probabil);
        if (TEST) printf("Natomiast, nie zmieniając wyboru wygrałem ");
        printf("%d ", non_m_h_wins);
        if (TEST) printf("razy. To jest [%%] ");
        printf("%.2f\n", non_m_h_probabil);
        break;
    default:
        printf("Numer testu spoza zakresu [1, 5] %d", to_do);
        break;
    }
    return 0;
}
