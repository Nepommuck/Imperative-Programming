#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define TEST 1  // 1 - dla testowania,  0 - dla automatycznej oceny

#define KEYW_NUM 32

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int is_letter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        return 1;
    return 0;
}

int is_digit(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

int not_a_keyword(int ind) {
    for (int i=0; i<KEYW_NUM; i++)
        if ( strcmp(keywords[i], tab[ind]) == 0 )
            return 0;
    return 1;
}

int new_id(int ind) {
    for (int i=0; i<ind; i++)
        if ( strcmp(tab[i], tab[ind]) == 0 )
            return 0;
    return 1;
}

int find_idents(FILE *stream)
{
    int odp = 0;
    int i = 0;   // index of world
    int k = 0;   // index of letter
    int mode = 0;
    // 0 - normal
    // 1 - block comment
    // 2 - line comment
    // 3 - IN ID
    // 4 - ''
    // 5 - ""

    char c = getc(stream);
    while (c != EOF)
    {
        // start comment
        if (mode == 0 && c == '/') {
            char next = getc(stream);
            if (next == '/')
                mode = 2;
            else if (next == '*')
                mode = 1;
        }
        // end block comment
        else if (mode == 1 && c == '*') {
            char next = getc(stream);
            if (next == '/')
                mode = 0;
        }
        // end line comment
        else if (mode == 2 && c == '\n')
            mode = 0;
        // strings and chars
        else if (mode == 0 && c == '\'')
            mode = 4;
        else if (mode == 0 && c == '"')
            mode = 5;
        // pass
        else if ((mode == 4 || mode == 5) && c == '\\')
            c = getc(stream);
        else if (mode == 4 && c == '\'' || mode == 5 && c == '"')
            mode = 0;

        // start of ID
        else if (mode == 0 && is_letter(c)) {
            tab[i][k] = c;
            k++;
            mode = 3;
        }
        // continue ID
        else if (mode == 3 && (is_letter(c) || is_digit(c))) {
            tab[i][k] = c;
            k++;
        }
        // end ID
        else if (mode == 3) {
            printf("Found: ");
            for (int j=0; j<k; j++)
                printf("%c", tab[i][j]);
            printf("\n");
            tab[i][k] = '\0';
            if(not_a_keyword(i) && new_id(i)) {

                printf("approved\n");
                i++;
                odp++;
            }
            k = 0;
            mode = 0;
        }

        c = getc(stream);
    }

    return odp;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
    char c = '\\';
    char file_name[40];
	FILE *stream;

  if(TEST)  stream = stdin;
  else {
      scanf("%s",file_name);
      stream = fopen(file_name,"r");
      if(stream == NULL) {
          printf("fopen failed\n");
          return -1;
      }
  }
	printf("%d\n", find_idents(stream));
	return 0;
}
