#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define FILE_OPEN_ERROR          -2

#define TEST 1  // 1 dla testowania, 0 dla automatycznej oceny

//###################### deklaracja struktur
// utworzenie wêz³a dla teblicy
typedef struct Node
{
    int *data;
    int len;
    struct Node* next;
    struct Node* prev;
} Node;

// utworzenie tagów listy
typedef struct tagList
{
    Node *first;
    Node *last;
    int size;
} List;


// utworzenie węzła dla teblicy znaków
typedef struct strNode
{
    char first_letter;
    char **data;
    int len;
    struct strNode* next;
    struct strNode* prev;
} strNode;

// utworzenie tagów listy znaków
typedef struct strtagList
{
    strNode *first;
    strNode *last;
    int size;
} strList;


/* inicjalizacja listy */
List* create_list()
{
    List* lis = malloc( sizeof(List) );
    lis->first=NULL;
    lis->last=NULL;
    lis->size=0;
    return lis;
}

Node* create_node(int arr[], int n) {
    Node* nod = malloc( sizeof(Node) );
    int* arr2 = malloc( n * sizeof(int) );
    memcpy(arr2, arr, n * sizeof(int));
    nod -> data = arr;
    nod -> len = n;
    return nod;
}

strList* s_create_list()
{
    strList *lis = malloc( sizeof(strList) );
    lis -> first = NULL;
    lis -> last = NULL;
    lis -> size=0;
    return lis;
}

strNode* s_create_node(char letter, char** words) {
    strNode* nod = malloc( sizeof(strNode) );
    nod -> first_letter = letter;
    nod -> data = words;
    nod -> len = 1;
    nod -> prev = NULL;
    nod -> next = NULL;
    return nod;
}


//###################### kod do uzupe³nienia(zadania)

//### zadanie 1
// wstawienie elementu na koñcu listy dwukierunkowej
void push_back(List *list, int *data, int len) {
    Node* nod = create_node(data, len);
    nod -> prev = list -> last;
    if (list -> last != NULL)
        list -> last -> next = nod;
    list -> last = nod;
    if (list -> first == NULL)
        list -> first = nod;
    list -> size++;
}



//### zadanie 2

// iteracja do przodu
int get_forw(List *list, int num)
{
    num -= 1;
    Node* p = list -> first;
    while (p->next != NULL && num >= p->len) {
        num -= (p->len);
        p = p->next;
        // printf("num %d \n", num);
    }
    return (p->data)[num];
}



//### zadanie 3
// iteracja do ty³u

int get_back(List *list, int num)
{
    num -= 1;
    Node* p = list -> last;
    while (p->prev != NULL && num >= p->len) {
        num -= (p->len);
        p = p->prev;
        // printf("num %d \n", num);
    }
    return (p->data)[p->len - num - 1];
}



//### zadanie 4
// usuwanie elementu, je¿eli jest to ostatni element w tablicy to usuwa element listy
void remove_at(List *list, int num)
{
    num -= 1;
    Node* p = list -> first;
    while (p->next != NULL && num >= p->len) {
        num -= (p->len);
        p = p->next;
        // printf("num %d \n", num);
    }

    if (p->len > 1) {
        // int n_arr[p->len - 1];
        int* n_arr = malloc( (p->len) * sizeof(int) );

        for (int i=0; i<num; i++)
            n_arr[i] = p->data[i];
        for (int i=num+1; i<p->len; i++)
            n_arr[i-1] = p->data[i];

        free(p->data);

        p->data = n_arr;
        p->len--;
    }
    else {
        if (p->prev != NULL)
            p->prev->next = p->next;
        else
            list->first = p->next;
        if (p->next != NULL)
            p->next->prev = p->prev;
        else
            list->last = p->prev;

        free(p->data);
        free(p);

        list->size--;
    }
}


//### zadanie 5
// wstawienie s³owa do komórki
void put_word_to_cell(strList *list, char *data) {
/*
    char* n_data = malloc(strlen(data) + 1);
    for (int i=0; i < strlen(data) + 1; i++)
        n_data[i] = data[i];
    // memcpy(n_data, data, strlen(data) + 1);
*/
    char** ptr = malloc( sizeof(char*) );
    *ptr = malloc(strlen(data) + 1);
    for (int i=0; i < strlen(data) + 1; i++)
        (*ptr)[i] = data[i];


    // printf("ok \n");
    // Empty list
    if (list->size == 0) {
        // printf("empt \n");
        list->first = s_create_node(data[0], ptr);
        list->last = list->first;
        list->size++;
        // printf("%s \n", *(list->first->data));
        // sdumpList(list);
        return;
    }
    // New first
    if (list->first->first_letter > data[0]) {
        strNode* q = s_create_node(data[0], ptr);
        q->next = list->first;
        list->first->prev = q;

        list->first = q;
        list->size++;
        return;
    }

    strNode* p = list->first;
    while (p->next != NULL && p->next->first_letter < data[0])
        p = p->next;

    // New node
    if (p->next == NULL || p->next->first_letter > data[0]) {
        // printf("nnode \n");
        // printf("%s \n", *(list->first->data));
        // printf("%s \n",*(p->data));
        strNode* q = s_create_node(data[0], ptr);
        q->prev = p;
        q->next = p->next;

        p->next = q;
        if (q->next != NULL)
            q->next->prev = q;

        list->size++;
        // sdumpList(list);
        return;
    }
    // Enlarge existing node
    p = p->next;
    p->len++;
    p->data = realloc(p->data, sizeof(char*)*(p->len));
    p->data[p->len-1] = *ptr;
}

int comp(const void* a, const void* b) {
    char** s1 = (char**) a;
    char** s2 = (char**) b;

    int rez =  strcmp(*s1, *s2);
    // printf("%s  %d  %s \n", *s1, rez, *s2);
    return rez;
}

void sort_list(strList* list) {
    strNode* p = list->first;
    while (p != NULL) { /*
        printf("Sorting: \n");
        for (int i=0; i< p->len; i++)
            printf("%s  ", p->data[i]);
        printf("\n"); */
        int size = sizeof (p->data) / sizeof *(p->data);
        qsort(p->data, p->len, sizeof(char*), comp);

        p = p->next;
    }
}

//###################### funkcje pomocnicze

/* Wypisanie zawartoœci listy, iteracja po elementach listy */
void dumpList(const List *list)
{
    Node *i;
    for(i=list->first; i!=NULL; i=i->next)
    {
        printf("-> ");
        for(int k=0; k<i->len; k++)
        {
            if(k<i->len-1)
            {
                printf("%d, ",i->data[k]);
            }
            else
            {
                printf("%d ",i->data[k]);
            }
        }
    }
    printf("\n");
}


// Wypisanie zawartoœci listy, iteracja po elementach listy

void sdumpList(const strList *list)
{
    strNode *i;
    for(i=list->first; i!=NULL; i=i->next)
    {
        printf("-> ");
        for(int k=0; k<i->len; k++)
        {
            if(k<i->len-1)
            {
                printf("%s, ",i->data[k]);
            }
            else
            {
                printf("%s ",i->data[k]);
            }
        }
    }
    printf("\n");
}

/* Usuwanie pierwszego elementu */
void delete_front(List *list)
{
    Node *toDelete;
    if(list->first==NULL)return;
    toDelete = list->first;
    list->first=list->first->next;
    if(list->first==NULL) list->last=NULL;
    free(toDelete);
    list->size--;
}

/* Usuwanie pierwszego elementu str */

void strdelete_front(strList *list) {
    strNode *toDelete;
    if(list->first==NULL)return;
    toDelete = list->first;
    list->first=list->first->next;
    if(list->first==NULL) list->last=NULL;
    for(int i=0; i<toDelete->len; i++)
    {
        free(toDelete->data[i]);
    }
    free(toDelete->data);
    free(toDelete);
    list->size--;
}

void stream_to_listoftables(strList *list, FILE *stream)
{

    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-\"";
    while(fgets(buf,BUFFER_SIZE,stream))
    {
        p = strtok(buf, delimits);
        while(p)
        {
            // printf("ok: %s \n", p);
            put_word_to_cell(list, p);
            p = strtok(NULL, delimits);
        }
    }
}

/* Zwalnianie ca³ej listy */
void freeList(List *list)
{
    while(list->first)
    {
        delete_front(list);
    }
}

/* Zwalnianie ca³ej listy str*/


void strfreeList(strList *list)
{
    while(list->first)
    {
        strdelete_front(list);
    }
}


// odczyt wektora wartoœci ca³kowitych
void read_vector(int tab[], int len)
{
    for (int i = 0; i < len; ++i)
    {
        scanf("%d", tab + i);
    }
}


int main()
{
    int to_do, n, len, tab[100];
    FILE *stream;

    List* list = create_list();
    strList* slist = s_create_list();
    // strList slist;

    if (TEST) printf("Wpisz numer testu [1, 5]: ");
    scanf("%d", &to_do);

    switch (to_do)
    {
    case 1:

        if (TEST) printf("wprowadŸ liczbê wêz³ów: ");
        scanf("%d", &n);
        for (int i=0; i<n; i++)
        {
            if (TEST) printf("podaj d³ugoœæ tablicy %d: ", i+1);
            scanf("%d", &len);
            if (TEST) printf("podaj elementy tablicy %d: ", i+1);
            int *tab = (int*) malloc(len*sizeof(int));
            read_vector(tab, len);
            // printf("a \n ");
            push_back(list, tab, len);
            // printf("a \n ");
        }

        dumpList(list);
        freeList(list);
        break;
    case 2:

        if (TEST) printf("wprowadŸ liczbê wêz³ów: ");
        scanf("%d", &n);
        for (int i=0; i<n; i++)
        {
            if (TEST) printf("podaj d³ugoœæ tablicy %d: ", i+1);
            scanf("%d", &len);
            if (TEST) printf("podaj elementy tablicy %d: ", i+1);
            int *tab = (int*) malloc(len*sizeof(int));
            read_vector(tab, len);
            push_back(list, tab, len);
        }
        if (TEST) dumpList(list);

        if (TEST) printf("wprowadŸ liczbê elementów do znalezienia: ");
        scanf("%d", &len);
        if (TEST) printf("wprowadŸ numery wybranych elementów: ");
        read_vector(tab, len);

        for (int i=0; i<len; i++)
        {
            printf("%d ", get_forw(list, tab[i]));
        }

        freeList(list);
        break;
    case 3:

        if (TEST) printf("wprowadŸ liczbê wêz³ów: ");
        scanf("%d", &n);
        for (int i=0; i<n; i++)
        {
            if (TEST) printf("podaj d³ugoœæ tablicy %d: ", i+1);
            scanf("%d", &len);
            if (TEST) printf("podaj elementy tablicy %d: ", i+1);
            int *tab = (int*) malloc(len*sizeof(int));
            read_vector(tab, len);
            push_back(list, tab, len);
        }
        if (TEST) dumpList(list);

        if (TEST) printf("wprowadŸ liczbê elementów do znalezienia: ");
        scanf("%d", &len);
        if (TEST) printf("wprowadŸ numery wybranych elementów: ");
        read_vector(tab, len);
        for (int i=0; i<len; i++)
        {
            printf("%d ", get_back(list, tab[i]));
        }

        freeList(list);
        break;
    case 4:

        if (TEST) printf("wprowadŸ liczbê wêz³ów: ");
        scanf("%d", &n);
        for (int i=0; i<n; i++)
        {
            if (TEST) printf("podaj d³ugoœæ tablicy %d: ", i+1);
            scanf("%d", &len);
            if (TEST) printf("podaj elementy tablicy %d: ", i+1);
            int *tab = (int*) malloc(len*sizeof(int));
            read_vector(tab, len);
            push_back(list, tab, len);
        }
        if (TEST) dumpList(list);

        if (TEST) printf("wprowadŸ liczbê komórek do usuniêcia: ");
        scanf("%d", &len);
        if (TEST) printf("wprowadŸ numery komórek do usuniêcia: ");
        read_vector(tab, len);
        for (int i=0; i<len; i++)
        {
            remove_at(list, tab[i]);
        }

        dumpList(list);
        freeList(list);

        break;
    case 5:

        if(TEST)
        {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_listoftables(slist, stream);
            rewind(stream);
        }
        else
        {
            stream = fopen("Tekst_1b.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_listoftables(slist, stream);
            fclose(stream);
        }
        sort_list(slist);
        sdumpList(slist);
        strfreeList(slist);
        break;
    default:
        printf("Numer testu spoza zakresu [1, 5] %d", to_do);
        break;
    }
    return 0;
}
