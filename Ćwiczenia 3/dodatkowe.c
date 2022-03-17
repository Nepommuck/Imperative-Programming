#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define L_KOSTEK  5                 //Liczba kostek ktorymi rzucami - dla polecenia ustawic 2
#define WIDTH  75                             //Maksymalna szerokosc reprezentacji graficznej
#define L_PROB  ( 100*1000 )                                      //Liczba prob do algorytmow
#define MOZ_SUM  ( 6 * L_KOSTEK - L_KOSTEK + 1 )  //Liczba mozliwych sum przy rzucie kostkami


void rep_graf(double tab[], int n, int przes)
{
    double mx = tab[0];
    for (int i=1; i<n; i++)
        if (tab[i] > mx)
            mx = tab[i];
    double przel = (WIDTH - 11) / mx;

    for (int i=0; i<n; i++)
    {
        printf("%2.d |", i+przes);
        for (int k=1; k<tab[i]*przel; k++)
            printf("*");
        printf("  %.1f%% \n", tab[i]);
    }
}

void probab_distrib(double v[], int trials_num)
{
    int a;
    for (int i=0; i<MOZ_SUM; i++)
        v[i] = 0;
    for (int i=0; i<trials_num; i++)
    {
        a = 0;
        for (int k=0; k<L_KOSTEK; k++)
            a += (rand() % 6);
        v[a]++;
    }
    for (int i=0; i<MOZ_SUM; i++)
        v[i] = v[i] * 100 / trials_num;
}

void var_dystryb(double dystr[], int len)
{
    probab_distrib(dystr, len);
    for (int i=MOZ_SUM-1; i>=0; i--)
        for (int k=0; k<i; k++)
            dystr[i] += dystr[k];
}

void odmien_kost(int ile)
{
    if (ile==1)
        printf("KOSTKA");
    else if (ile%10 >= 2 && ile%10 <= 4 && (ile < 10 || ile > 20))
        printf("KOSTKI");
    else
        printf("KOSTEK");
}

int main()
{
    srand(time(0));

    double v[MOZ_SUM];
    double w[MOZ_SUM];
    probab_distrib(v, L_PROB);
    var_dystryb(w, L_PROB);

    printf(" [%d ", L_KOSTEK);
    odmien_kost(L_KOSTEK);
    printf("]\n\n");
    printf(" Dyskretny rozklad prawdopodobienstwa: \n");
    rep_graf(v, MOZ_SUM, L_KOSTEK);
    printf("\n\n Dystrybuanta: \n");
    rep_graf(w, MOZ_SUM, L_KOSTEK);

    return 0;
}
