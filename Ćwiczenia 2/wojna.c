#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MODE 0    // 0 - wersja zgodna z poleceniem
                  // 1 - wersja "user friendly"

#define L_KOLOROW  4
#define CBUFF_SIZE  ( L_KOLOROW*13 )

int rand_from_interval(int a, int b)
{
    if (a > b)
        return INT_MIN;
    if (b - a > RAND_MAX)
        return INT_MIN;
    if (a == b)
        return a;
    return (rand() % (b-a+1)) + a;
}

void rand_permutation(int n, int tab[])
{
    for (int i=0; i<n; i++)
        tab[i] = i;
    for (int i=0; i<n-1; i++)
    {
        int k = rand_from_interval(i, n-1);
        int tmp = tab[i];
        tab[i] = tab[k];
        tab[k] = tmp;
    }
}

int cbuff_push(int cbuff[], int out, int *len, int karta)
{
    if (*len >= CBUFF_SIZE)
        return -1;
    cbuff[(out+*len)%CBUFF_SIZE] = karta;
    (*len)++;
    return 0;
}

int cbuff_pop(int cbuff[], int *out, int *len)
{
    if (*len == 0)
        return -1;
    (*len)--;
    int ret = cbuff[*out];
    *out = (*out + 1) % CBUFF_SIZE; //*out++;
    return ret;
}

// Wypisuje elementy listy w buforze cyklicznym
void cbuff_state(int cbuff[], int out, int len)
{
    if (len == 0)
        printf("Lista pusta \n");
    else
    {
        for (int i=0; i<len; i++)
            printf("%2.d ", cbuff[(out+i)%CBUFF_SIZE]);
        printf("\n");
    }
}

// Wypisuje karty (z pominięciem koloru) jakie posiada dany gracz
void print_player(int cbuff[], int out, int len)
{
    if (len == 0)
        printf("Talia pusta \n");
    else
    {
        // 'd' = '10'
        char figury[] = {'d', 'J', 'Q', 'K', 'A'};
        for (int i=0; i<len; i++)
        {
            int karta = cbuff[(out+i)%CBUFF_SIZE]/L_KOLOROW + 2;
            char symb;
            if (karta < 10)
                symb = ('0' + karta);
            else
                symb = figury[karta - 10];

            printf("%c ", symb);
        }
        printf("\n");
    }
}

void odmien_kart(int ile)
{
    if (ile==1)
        printf("karta");
    else if (ile%10 >= 2 && ile%10 <= 4 && (ile < 10 || ile > 20))
        printf("karty");
    else
        printf("kart");
}

// Wyswietla obecny stan rozgrywki
void game_state(int grA, int outA, int lenA, int grB, int outB, int lenB)
{
    printf("Gracz A - %d ", lenA);
    odmien_kart(lenA);
    printf(": \n");
    print_player(grA, outA, lenA);
    printf("\nGracz B - %d ", lenB);
    odmien_kart(lenB);
    printf(": \n");
    print_player(grB, outB, lenB);
}

// Dodaje karte do puli kontestowanych w wyniku wojny
void push_kont(int kont[], int *len, int karta)
{
    kont[*len] = karta;
    (*len)++;
}

// Przekazuje pule kart kontestowanych graczowi ktory wygral wojne
// (talia gracza, out Gracza, dl. talii gracza, kontestowane karty, liczba kontestowanych kart)
void daj_graczowi(int cbuff[], int out, int *len, int kont[], int *lenKont)
{
    for (int i=0; i<(*lenKont); i++)
        cbuff_push(cbuff, out, len, kont[i]);
    *lenKont = 0;
}

// Pozwala ustalic starszenstwo
int moc_karty (int karta)
{
    return karta / L_KOLOROW;
}

int main()
{
    int seed, gamemode, max_konf;

    if (MODE == 1)
        printf("Podaj po spacji: ziarno, tryb gry [0 lub 1] oraz maksymalna liczbe konfliktow... \n");

    scanf("%d%d%d", &seed, &gamemode, &max_konf);
    srand(seed);

    // Cala talia przed rozdaniem oraz talie obu graczy podczas rozgrywki
    int talia[CBUFF_SIZE], grA[CBUFF_SIZE], grB[CBUFF_SIZE], outA=0, outB=0, lenA=CBUFF_SIZE/2, lenB=CBUFF_SIZE/2;

    // Zbior kart kontestowanych podczas wojny (kolejna niecykliczna)
    int kontest[CBUFF_SIZE], lenKont=0;

    // Rezultat (-1) oznacza grę w toku, liczba konfliktow, maksymalna dozwolona liczba konfliktow
    int rez=-1, l_konf=0;

    for (int i=0; i<CBUFF_SIZE; i++)
        talia[i] = i;

    rand_permutation(CBUFF_SIZE, talia);
    for (int i=0; i<(CBUFF_SIZE/2); i++)
        grA[i] = talia[i];
    for (int i=0; i<(CBUFF_SIZE/2); i++)
        grB[i] = talia[CBUFF_SIZE/2 + i];

    if (MODE == 1)
    {
        printf("\n");
        game_state(grA, outA, lenA, grB, outB, lenB);
        printf("\nZaczynamy! \n");
    }

    while (rez == -1)
    {
        l_konf++;

        int a = cbuff_pop(grA, &outA, &lenA);
        int b = cbuff_pop(grB, &outB, &lenB);

        // A wygrywa runde
        if (moc_karty(a) > moc_karty(b))
        {
            cbuff_push(grA, outA, &lenA, a);
            cbuff_push(grA, outA, &lenA, b);
        }
        // B wygrywa runde
        else if (moc_karty(a) < moc_karty(b))
        {
            cbuff_push(grB, outB, &lenB, a);
            cbuff_push(grB, outB, &lenB, b);
        }
        // WOJNA
        else if (gamemode == 0)
        {
            if (MODE == 1)
                printf("\nWojna!!! \n\n");

            push_kont(kontest, &lenKont, a);
            push_kont(kontest, &lenKont, b);
            int roztrzyg = 0;

            while (rez == -1 && !roztrzyg)
            {
                if (l_konf >= max_konf)
                    rez=0;
                else if (lenA < 2 || lenB < 2)
                    rez=1;
                else
                {
                    l_konf++;

                    // 2 zakryte karty
                    push_kont(kontest, &lenKont, cbuff_pop(grA, &outA, &lenA));
                    push_kont(kontest, &lenKont, cbuff_pop(grB, &outB, &lenB));

                    // 2 karty odkryte
                    a = cbuff_pop(grA, &outA, &lenA);
                    b = cbuff_pop(grB, &outB, &lenB);
                    push_kont(kontest, &lenKont, a);
                    push_kont(kontest, &lenKont, b);

                    //gracz A wygrywa wojnę
                    if (moc_karty(a) > moc_karty(b))
                    {
                        roztrzyg = 1;
                        daj_graczowi(grA, outA, &lenA, kontest, &lenKont);
                    }
                    //gracz B wygrywa wojnę
                    else if (moc_karty(b) > moc_karty(a))
                    {
                        roztrzyg = 1;
                        daj_graczowi(grB, outB, &lenB, kontest, &lenKont);
                    }
                }
            }
        }
        // Remis - wersja uproszczona
        else
        {
            cbuff_push(grA, outA, &lenA, a);
            cbuff_push(grB, outB, &lenB, b);
        }

        if (MODE == 1)
        {
            printf("\n");
            game_state(grA, outA, lenA, grB, outB, lenB);
            printf("\nNacisnij ENTER, aby kontynuowac... \n\n");
            system("pause>null");
        }

        if (rez == -1)
        {
            // wygrana gracza A
            if (lenB <= 0)
                rez = 2;
            // wygrana gracza B
            else if (lenA <= 0)
                rez = 3;
            // przekroczona liczba konfliktow
            else if (l_konf >= max_konf)
                rez=0;
        }
    }

    if (MODE == 1)
    {
        printf("\nRezultat gry: \n");
        switch (rez)
        {
            case 0:
                printf("Gra niedokonczona - przekroczono liczbe konfliktow");
                break;

            case 1:
                printf("Gra nieroztrzygnieta - zabraklo kart podczas wojny");
                break;

            case 2:
                printf("Zwyciestwo gracza A");
                break;

            case 3:
                printf("Zwyciestwo gracza B");
                break;

            default:
                printf("Blad programu");
        }
        printf("\n\n");
        printf("Liczba konfliktow: %d \n", l_konf);
        game_state(grA, outA, lenA, grB, outB, lenB);
    }
    else
    {
        printf("%d \n", rez);
        switch (rez)
        {
            case 0:
            case 1:
                printf("%d \n", lenA);
                printf("%d \n", lenB);
                break;

            case 2:
                printf("%d \n", l_konf);
                break;

            case 3:
                cbuff_state(grB, outB, lenB);
                break;

            default:
                printf("Fatal error \n");
        }
    }


    return 0;
}
