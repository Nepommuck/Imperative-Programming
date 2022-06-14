#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
	int int_data;
	char char_data;
	void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef struct hash_table {
	int size;          // liczba elementow tablicy
	int no_elements;
	ht_element **ht;   // tablica wskaźników na elementy
	DataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {

    p_table->ht = malloc( size * sizeof(ht_element*) );
    for (int i=0; i<size; i++)
        p_table->ht[i] = NULL;

    p_table->size = size;
    p_table->no_elements = 0;
    p_table->dump_data = dump_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
    ht_element* p = (p_table->ht)[n];

    while (p != NULL) {
        (p_table->dump_data)(p->data);
        p = p->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {

    if (free_data != NULL)
        (free_data)((to_delete->data));
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    for (int i=0; i < p_table->size; i++) {
        ht_element* p = p_table->ht[i];
        ht_element* prev = NULL;
        while (p != NULL) {
            // printf("(%d, %d) \n", i, p->data.int_data);
            prev = p;
            p = p-> next;
            free_element(p_table->free_data, prev);
        }
    }
    // p_table is not a pointer!
    // free(p_table);
}

// calculate hash function for integer k
int hash_base(int k, int size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (int)floor(size * (tmp - floor(tmp)));
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {

}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
  int hash = p_table->hash_function(*data, p_table->size);
  ht_element *e = p_table->ht[hash];
  while (e) {
    if (p_table->compare_data(e->data, *data) == 0) {
      return e;
    }
    e = e->next;
  }
  return NULL;
}

void insert_element(hash_table *p_table, data_union data);

void rehash(hash_table *p_table) {
    int o_size = p_table->size;
    ht_element** old_tab = p_table->ht;

    p_table->size = 2 * o_size;
    p_table->ht = malloc( p_table->size * sizeof(ht_element*) );
    for (int i=0; i<p_table->size; i++)
        p_table->ht[i] = NULL;
    p_table->no_elements = 0;


    for (int i=0; i < o_size; i++) {
        ht_element* p = old_tab[i];
        ht_element* prev = NULL;
        while (p != NULL) {
            prev = p;
            p = p-> next;

            insert_element(p_table, prev->data);
            free(prev);
        }
        // printf("ok \n");
    }
    free(old_tab);
}

// insert element
void insert_element(hash_table *p_table, data_union data) {
    int ind = (p_table->hash_function)(data, p_table->size);

    ht_element* p = p_table->ht[ind];
    // Checking if element not already here
    while (p != NULL) {
        if ( (p_table->compare_data)(p->data, data) == 0 )
            return;
        p = p->next;
    }

    p = malloc( sizeof(ht_element) );
    p->data = data;
    p->next = p_table->ht[ind];
    p_table->ht[ind] = p;
    p_table->no_elements++;

    if (p_table->no_elements > MAX_RATE * p_table->size)
        rehash(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    int ind = (p_table->hash_function)(data, p_table->size);

    ht_element* p = p_table->ht[ind];
    // Should not happen
    if (p == NULL)
        return;
    // Remove first element
    //  (p->data == data)
    if ((p_table->compare_data)(p->data, data) == 0) {
        p_table->ht[ind] = p->next;
        free(p);
    }

    //                        (p->next->data != data)
    while (p->next != NULL && (p_table->compare_data)(p->next->data, data) != 0)
        p = p->next;
    if (p->next != NULL) {
        ht_element* q = p->next;
        p->next = q->next;
        free(q);
    }
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
    data_union data;
    scanf("%d", &data.int_data);
    return data;
}

// char element

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return a.char_data - b.char_data;
}

// read char value and insert to the union
data_union create_data_char() {
    data_union data;
    scanf(" %c", &data.char_data);
    return data;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void str_lower(char* str) {
    while (*str != '\0') {
        *str = tolower(*str);
        str++;
    }
}

void dump_word(data_union data) {
    printf("%s %d", ((DataWord*)(data.ptr_data))->word, ((DataWord*)(data.ptr_data))->counter);
}

void free_word(data_union data) {
    free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
    return strcmp(((DataWord*)(a.ptr_data))->word, ((DataWord*)(b.ptr_data))->word);
}

int hash_word(data_union data, int size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

// increase the counter
void modify_word(data_union* data) {
    ((DataWord*)(data.ptr_data))->counter++;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
    data_union p;
    p.ptr_data = malloc( sizeof(DataWord) );

    // p.word = malloc(BUFFER_SIZE);
    ((DataWord*)(p.ptr_data))->word = value;
    ((DataWord*)(p.ptr_data))->counter = 1;
    return p;
}


// insert word
void insert_word(hash_table *p_table, data_union data) {
    int ind = (p_table->hash_function)(data, p_table->size);

    ht_element* p = p_table->ht[ind];
    // Checking if element not already here
    while (p != NULL) {
        if ( (p_table->compare_data)(p->data, data) == 0 ) {
            modify_word( &(p->data) );
            return;
        }
        p = p->next;
    }

    p = malloc( sizeof(ht_element) );
    p->data = data;
    p->next = p_table->ht[ind];
    p_table->ht[ind] = p;
    p_table->no_elements++;


    if (p_table->no_elements > MAX_RATE * p_table->size) {
        rehash(p_table);
    }
}


// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char line[BUFFER_SIZE];
    while ( fgets(line, BUFFER_SIZE, stream) ) {
        //printf("%s \n", line);
        char* ob = strtok(line, " \n\r.,?!:;-");
        while (ob != NULL) {
            //printf("%s \n", ob);
            char* word = strdup(ob);
            str_lower( word );
            data_union data = create_data_word( word );
            // printf("Created: %s\n", ob);
            insert_word(p_table, data);
            ob = strtok(NULL, " \n\r.,?!:;-");
        }
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = create_data();
		// printf("ok \n");
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

void debug_int(hash_table *p_table) {
    printf("List of %d size: \n", p_table->size);
    for (int i=0; i<p_table->size; i++) {
        printf("[%d]: ", i);
        ht_element* p = p_table->ht[i];
        while (p != NULL) {
            printf("%d ", p->data.int_data);
            p = p->next;
        }

        printf("\n");
    }
    printf("\n\n");
}

int main(void) {
	int to_do, n, index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

/*
	// DEBUG
    init_ht(&table, 4, dump_int, NULL, cmp_int, hash_int, NULL);
	while (1) {
        // scanf("%d", &n);
        data_union dt = create_data_int();
        insert_element(&table,  dt);
        debug_int(&table);
	}
*/
	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_int,NULL,
					cmp_int, hash_int, NULL);
			table_test(&table, n, create_data_int);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			// printf("ok \n");
			free_table(&table);

			break;
		case 2: // test char table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_char,NULL,
			        cmp_char, hash_char, NULL);
			table_test(&table, n, create_data_char);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 3: // read words from text, insert into table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, free_word,
			        cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%d\n", table.size);
			data = create_data_word(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
