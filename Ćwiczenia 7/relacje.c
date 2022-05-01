#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;


// function to swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(int array[], int low, int high) {
  int pivot = array[high];
  int i = (low - 1);
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }

  swap(&array[i + 1], &array[high]);
  return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {
    int pi = partition(array, low, high);
    quickSort(array, low, pi - 1);
    quickSort(array, pi + 1, high);
  }
}


// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair relation[], int size) {
    int num;
    for (int i=0; i<2*size; i++) {
        if (i%2 == 0)
            num = relation[i/2].first;
        else
            num = relation[i/2].second;
        int ok = 0;
        for (int k=0; k<size; k++)
            if (relation[k].first == num && relation[k].first == relation[k].second) {
                ok = 1;
                break;
            }
        if (!ok)
            return 0;
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair relation[], int size) {
    int num;
    for (int i=0; i<2*size; i++) {
        if (i%2 == 0)
            num = relation[i/2].first;
        else
            num = relation[i/2].second;
        int ok = 0;
        for (int k=0; k<size; k++)
            if (relation[k].first == num && relation[k].first == relation[k].second)
                return 0;
    }
    return 1;
}


// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair relation[], int size) {
    int num, a, b, ok;
    for (int i=0; i<size; i++) {
        a = relation[i].first;
        b = relation[i].second;
        ok = 0;
        for (int k=0; k<size; k++)
            if (relation[k].first == b && relation[k].second == a) {
                ok = 1;
                break;
            }
        if (!ok)
            return 0;
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair relation[], int size) {
    int num, a, b, ok;
    for (int i=0; i<size; i++) {
        a = relation[i].first;
        b = relation[i].second;
        if (a != b)
            for (int k=0; k<size; k++)
                if (relation[k].first == b && relation[k].second == a) {
                    return 0;
                }
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair relation[], int size) {
    int num, a, b, ok;
    for (int i=0; i<size; i++) {
        a = relation[i].first;
        b = relation[i].second;
        for (int k=0; k<size; k++)
            if (relation[k].first == b && relation[k].second == a) {
                return 0;
            }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair relation[], int size) {
    int num, a, b, c;
    for (int i=0; i<size; i++) {
        a = relation[i].first;
        b = relation[i].second;
        for (int k=0; k<0; k++) {
            if (relation[k].first == b) {
                c = relation[k].second;
                int ok = 0;
                for (int j=0; j<size; j++)
                    if (relation[j].first == a && relation[j].second == c) {
                        ok = 1;
                        break;
                    }
                if (!ok)
                    return 0;
            }
        }
    }
    return 1;
}


// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair rel[], int size)
{
    if(is_reflexive(rel, size) && is_antisymmetric(rel, size) && is_transitive(rel, size))
        return 1;
    return 0;
}

int get_domain(pair rel[], int size, int dziedz[]) {
    int dom[MAX_REL_SIZE], l=0;
    for (int i=0; i<size; i++) {
        dom[l++] = rel[i].first;
        dom[l++] = rel[i].second;
    }
    quickSort(dom, 0, l-1);
    dziedz[0] = dom[0];
    int siz_dzie = 1;
    for (int i=1; i<l; i++) {
        if (dom[i] != dziedz[siz_dzie-1])
            dziedz[siz_dzie++] = dom[i];
    }
    return siz_dzie;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair rel[], int size) {
    int dom[MAX_REL_SIZE];
    int dom_size = get_domain(rel, size, dom);

    for (int i=0; i<dom_size; i++)
        for (int k=i+1; k<dom_size; k++) {
            int a = dom[i], b = dom[k];
            int ok = 0;

            for (int j=0; j<size; j++)
                if(rel[j].first == a && rel[j].first == b || rel[j].first == b && rel[j].first == a) {
                    ok = 1;
                    break;
                }
            if (!ok)
                return 0;
        }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair rel[], int size)
{
    if(is_partial_order(rel, size) && is_connected(rel, size))
        return 1;
    return 0;
}

int find_max_elements(pair rel[], int size, int max_elems[]) {
    int dom[MAX_REL_SIZE];
    int dom_size = get_domain(rel, size, dom), max_size=0;

    for (int i=0; i<dom_size; i++) {
        int ok = 1;
        for (int k=0; k<size; k++)
            if (rel[k].first == dom[i] && rel[k].second != dom[i]) {
                ok = 0;
                break;
            }
        if (ok)
            max_elems[max_size++] = dom[i];
    }
    return max_size;
}

int find_min_elements(pair rel[], int size, int min_elems[]) {
    int dom[MAX_REL_SIZE];
    int dom_size = get_domain(rel, size, dom), min_size=0;

    for (int i=0; i<dom_size; i++) {
        int ok = 1;
        for (int k=0; k<size; k++)
            if (rel[k].first != dom[i] && rel[k].second == dom[i]) {
                ok = 0;
                break;
            }
        if (ok)
            min_elems[min_size++] = dom[i];
    }
    return min_size;
}

// Case 3:
int composition (pair rel1[], int size1, pair rel2[], int size2, pair comp[]) {
    int comp_size = 0, a, b;
    for (int i=0; i<size1; i++)
        for (int k=0; k<size2; k++)
            if (rel1[i].second == rel2[k].first) {
                a = rel1[i].first;
                b = rel2[k].second;
                int ok = 1;
                for (int j=0; j<comp_size; j++)
                    if (comp[j].first == a && comp[j].second == b) {
                        ok = 0;
                        break;
                    }
                if (ok) {
                    comp[comp_size].first = a;
                    comp[comp_size].second = b;
                    comp_size++;
                }
            }
    return comp_size;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Add pair to existing relation if not already there
int add_relation (pair relation[], int size, pair new_rel)
{
    for (int i=0; i<size; i++)
        if (relation[i].first == new_rel.first && relation[i].second == new_rel.second)
            return 0;
    relation[size].first = new_rel.first;
    relation[size].second = new_rel.second;
    return 1;
}


// Read number of pairs, n, and then n pairs of ints
int read_relation(pair relation[])
{
    int pairs_num, size=0, a, b;
    pair neww;

    scanf("%d", &pairs_num);
    for (int i=0; i<pairs_num; i++) {
        scanf("%d%d", &a, &b);
        neww.first = a;
        neww.second = b;
        size += add_relation(relation, size, neww);
    }
    return size;
}


void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
