#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

#define TEST 0   // 1 dla testowania, 0 dla automatycznej oceny

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stream
void wc(int *nl, int *nw, int *nc, FILE *stream)
{
    char c = getc(stream);
    *nc = 0;
    *nw = 0;
    *nl = 0;

    int ob_sl = 0;
    while (c != EOF)
    {
        (*nc)++;
        if (ob_sl && isspace(c) ){
            (*nw)++;
            ob_sl = 0;
        }
        else if (!ob_sl && !isspace(c) )
            ob_sl = 1;
        if (c == '\n')
            (*nl)++;
        // printf("k");
        c = getc(stream);
    }
}

void swapp(int tab[], int i, int k) {
    int temp = tab[i];
    tab[i] = tab[k];
    tab[k] = temp;
}

void quick_sort(int indeks[], int wyst[], int p, int k) {
    if (p < k) {
        int a = p;
        for (int i=p; i<k; i++) {
            if (wyst[indeks[i]] > wyst[indeks[k]] || (wyst[indeks[i]] == wyst[indeks[k]]) && i < k )  {
                swapp(indeks, a, i);
                a += 1;
            }
        }
        swapp(indeks, a, k);
        quick_sort(indeks, wyst, p, a-1);
        quick_sort(indeks, wyst, a+1, k);
    }
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stream. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt, FILE *stream)
{
    int n = LAST_CHAR - FIRST_CHAR;
    int chars[LAST_CHAR - FIRST_CHAR];
    for (int i=0; i<n; i++)
        chars[i] = 0;

    char c = getc(stream);
    while (c != EOF) {
        if (c >= FIRST_CHAR && c < LAST_CHAR)
            chars[c - FIRST_CHAR]++;
        c = getc(stream);
    }
    /*
    for (int i=0; i<n; i++)
        printf("[%d] '%c': %d \n", i, FIRST_CHAR+i, chars[i]);
    */

    int ind[LAST_CHAR - FIRST_CHAR];
    for (int i=0; i<n; i++)
        ind[i] = i;

    quick_sort(ind, chars, 0, n-1);
    /*
    for (int i=0; i<n; i++)
        printf("[%d] %d: '%c' - %d \n", i, ind[i], FIRST_CHAR+ind[i], chars[ind[i]]);
    */

    *n_char = ind[char_no - 1] + FIRST_CHAR;
    *cnt = chars[ind[char_no - 1]];

}

void dig_qsort(int indeks[], int wyst[][LAST_CHAR - FIRST_CHAR], int p, int k, int n) {
    if (p < k) {
        int a = p;
        for (int i=p; i<k; i++) {
            if (wyst[indeks[i] / n][indeks[i] % n] > wyst[indeks[k] / n][indeks[k] % n] ||
               (wyst[indeks[i] / n][indeks[i] % n] == wyst[indeks[k] / n][indeks[k] % n] &&
               (indeks[i]/n < indeks[k]/n || (indeks[i]/n == indeks[k]/n && indeks[i]%n < indeks[k]%n)) ) )
            {
                swapp(indeks, a, i);
                a += 1;
            }
        }
        swapp(indeks, a, k);
        dig_qsort(indeks, wyst, p, a-1, n);
        dig_qsort(indeks, wyst, a+1, k, n);
    }
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stream. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[], FILE *stream)
{
    int n = LAST_CHAR - FIRST_CHAR;
    int digrams[LAST_CHAR - FIRST_CHAR][LAST_CHAR - FIRST_CHAR];
    for (int i=0; i<n; i++)
        for (int k=0; k<n; k++)
            digrams[i][k] = 0;

    char prev = getc(stream);
    char c = getc(stream);
    while (c != EOF) {
        if (c >= FIRST_CHAR && c < LAST_CHAR && prev >= FIRST_CHAR && prev < LAST_CHAR)
            digrams[prev - FIRST_CHAR][c - FIRST_CHAR]++;
        prev = c;
        c = getc(stream);
    }
    int indeks[n*n];
    for (int i=0; i<n*n; i++)
        indeks[i] = i;
/*
    for (int i=0; i<n; i++)
        for (int k=0; k<n; k++)
            if (digrams[i][k] > 0)
                printf("%c%c:  %d \n", i+FIRST_CHAR, k+FIRST_CHAR, digrams[i][k]);
*/
    dig_qsort(indeks, digrams, 0, n*n-1, n);
/*
    for (int i=0; i<n*n; i++) {
        int a = indeks[i] / n;
        int b = indeks[i] % n;
        printf("%c%c:  %d \n", a+FIRST_CHAR, b+FIRST_CHAR, digrams[a][b]);
        if (digrams[a][b] == 0)
            break;
    }
*/
    int rez = indeks[digram_no - 1];
    digram[0] = rez/n + FIRST_CHAR;
    digram[1] = rez%n + FIRST_CHAR;
    digram[2] = digrams[rez/n][rez%n];

}

// Count block and line comments in the text read from stream. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter, FILE *stream)
{
    *line_comment_counter = 0;
    *block_comment_counter = 0;

    // 0 - normal
    // 1 - block comment
    // 2 - line comment
    int mode = 0;

    char c = 'a';
    while (c != EOF) {
        c = getc(stream);
        if (mode == 0 && c == '/') {
            char next = getc(stream);
            if (next == '/')
                mode = 2;
            else if (next == '*')
                mode = 1;
        }
        else if (mode == 1 && c == '*') {
            char next = getc(stream);
            if (next == '/') {
                mode = 0;
                (*block_comment_counter)++;
            }
        }
        else if (mode == 2 && c == '\n') {
            mode = 0;
            (*line_comment_counter)++;
        }
    }

}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	char file_name[40];
	FILE *stream;

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
    if(TEST)  stream = stdin;
    else {
        scanf("%s",file_name);
        stream = fopen(file_name,"r");
        if(stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }

//	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc, stream);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
//			char_no = read_line();
            if(TEST) printf("Wpisz numer znaku ");
            scanf("%d",&char_no);
			char_count(char_no, &n_char, &cnt, stream);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
//			char_no = read_line();
            if(TEST) printf("Wpisz numer digramu ");
            scanf("%d",&char_no);
			digram_count(char_no, digram, stream);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter, stream);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
