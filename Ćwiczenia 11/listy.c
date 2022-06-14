#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MEMORY_ALLOCATION_ERROR  -1


typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;


typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*,const void*);
typedef void (*InsertInOrder)(void*,void*);

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    int         size;
    ConstDataFp dumpData;  // Function to print an element
    DataFp      freeData;
    CompareDataFp compareData;
    InsertInOrder insertElem;
} List;


////////////////////////////////////////////////////////////////
// Funkcje obslugi listy

// wypisanie zawartosci elemntow listy
void dumpList(const List* list) {
    if(list->dumpData == NULL) return;
    ListElement *i;
    for(i=list->head; i!=0; i=i->next)
        list->dumpData(i->data);
}

// zwolnienie pamieci listy
void freeList(List*list) {
    ListElement *current = 0;
    current = list->head;
    while(current) {
        ListElement *todelete = current;
        current = current->next;
        if(list->freeData) list->freeData(todelete->data); // zwolnienie pamieci dla struktury data
        free(todelete);              // zwolnienie pamieci dla elementu
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

///////////////////////////////////////////////////////////

// Funkcje obslugi elementow listy

/////////////////// Do zadania 11.1.1 i nastepnych:

// dodaje element na poczatku listy
void pushFront(List *list, void *data){
    ListElement *element = (ListElement*)malloc(sizeof(ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = list->head;
    list->head = element;
    if(!list->tail) list->tail = list->head;
    list->size++;
}

// dodaje element na koncu listy
void pushBack(List *list, void *data) {
    ListElement *element = (ListElement*)malloc(sizeof(ListElement));
    if(element == NULL) exit(MEMORY_ALLOCATION_ERROR);
    element->data = data;
    element->next = NULL;
    if(list->tail) list->tail->next = element;
    list->tail = element;
    if(!list->head) list->head = list->tail;
    list->size++;
}

// usun pierwszy element listy
void popFront(List*list) {
    void *data;
    if(list->head){
        ListElement *todelete = list->head;
        if(list->tail == todelete) list->tail = NULL;
        list->head = list->head->next;
        if(list->freeData) list->freeData(todelete->data);
        free(todelete);
        list->size--;
    }
}

// odwraca kolejnosc elementow listy
void reverse(List *list) {
}

// wstawia element wg porzadku (okreslonym funkcja w naglowku listy w polu insertElem)
void insertInOrder(void *a, void *b) {
    List *plist = (List*)a;
    plist->insertElem(plist, b);
}

// Propozycje funkcji pomocniczych dla list uporzadkowanych


// FUNKCJE OGÓLNE
// zwraca element w posortowanej liscie, a jezeli nie ma, to element poprzedzajacy (nie ma, to NULL)
ListElement* findInsertionPoint(const List *list, ListElement *element) {
    CompareDataFp cmp = list -> compareData;

    // List is empty or element would be the smallest
    if(list -> head == NULL || cmp(list -> head, element))
        return NULL;

    ListElement* p = list -> head;
    while (p -> next != NULL && cmp(p -> next, element))
        p = p -> next;
    return p;
}

// dodaje element za wskazanym elementem - jezeli element NULL, to blad
//void pushAfter(List *list, void *data, ListElement *previous) {
void pushAfter(List *list, ListElement *new_el, ListElement *previous) {

    // At the beginning
    if (previous == NULL) {
        new_el -> next = list -> head;
        list -> head = new_el;
    }
    else {
        new_el -> next = previous -> next;
        previous -> next = new_el;
    }

    if (list -> tail == NULL)
        list -> tail = list -> head;

    else
        while (list -> tail -> next != NULL)
            list -> tail = list -> tail -> next;
}


/////////////////// Do zadania 11.1.2

// dodaje element na koncu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data) {
}

// Funkcje warstwy obslugi danych wskazywanych w elementach listy


//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dane: liczba typu int

typedef int DataInt;

void dump_int(const void *d) {
    printf("We are here \n");
    // ListElement *Le = (ListElement*) d;
    printf("%d", *((DataInt*) d) );
}

void free_int(void *d) {
}

int cmp_int(const void *a, const void *b) {
    ListElement *Le_a = (ListElement*) a;
    ListElement *Le_b = (ListElement*) b;

    DataInt *val_a = (DataInt*)(Le_a -> data);
    DataInt *val_b = (DataInt*)(Le_b -> data);

    return (*val_a) - (*val_b);
}

// dodaje element do posortowanej listy, jezeli taki element juz istnieje, to NTD (Nothing To Do)
//void addList_int(List *plist, int v) {

void insertElem_int(void *pl, void *a) {
    List* given_list = (List*) pl;
    ListElement* elem = (ListElement*) a;

    ListElement* ins_point =  findInsertionPoint(given_list, elem);
    pushAfter(given_list, elem, ins_point);
}

// inicjowanie listy
void init_List_int(List *p_list) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->size = 0;
    p_list->dumpData = dump_int;
    p_list->freeData = free_int;
    p_list->compareData = cmp_int;
    p_list->insertElem = insertElem_int;
}

// zaalokuj pamiêæ dla int
DataInt* create_data_int(int v) {
    DataInt* pointer = malloc( sizeof(DataInt) );
    *pointer = v;
    return pointer;
}

int main()
{
    List* my_list = malloc( sizeof(List) );
    init_List_int(my_list);

    int* v = malloc( 10 * sizeof(int) );
    v[0] = 2;
    v[1] = 8;


    dumpList(my_list);
    printf("\n");

    pushFront(my_list, &v);
    dumpList(my_list);
    printf("\n");

    pushFront(my_list, &v);

    dumpList(my_list);
    printf("\n");


    return 0;
}
