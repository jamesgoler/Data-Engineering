#include "compare.h"

int compare_numerical(void *a, void *b) {
    SortUnit *c = (SortUnit *)a;
    SortUnit *d = (SortUnit *)b;
    return (c->key.value.numerical > d->key.value.numerical);
}

int compare_numerical_rev(void *a, void *b) {
    SortUnit *c = (SortUnit *)a;
    SortUnit *d = (SortUnit *)b;
    return (c->key.value.numerical < d->key.value.numerical);
}

int compare_lexical(void *a, void *b) {
    SortUnit *c = (SortUnit *)a;
    SortUnit *d = (SortUnit *)b;
    return strcmp(c->key.value.lexical, d->key.value.lexical);
}

int compare_lexical_rev(void *a, void *b) {
    SortUnit *c = (SortUnit *)a;
    SortUnit *d = (SortUnit *)b;
    return strcmp(d->key.value.lexical, c->key.value.lexical);
}

int compare_size(void *a, void *b) {
    SortUnit *c = (SortUnit *)a;
    SortUnit *d = (SortUnit *)b;
    return (c->key.value.size > d->key.value.size);        
}

int compare_size_rev(void *a, void *b) {
    SortUnit *c = (SortUnit *)a;
    SortUnit *d = (SortUnit *)b;
    return (c->key.value.size < d->key.value.size);        
}