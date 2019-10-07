#include "sentence_count.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

extern int errno;

inline CounterList *new_counter_list(void) {
    CounterList *list = (CounterList *)malloc(sizeof(CounterList));
    list->num = 0;
    return list;
}

void insert(CounterList *self, uchar *slice_start, uchar *slice_end) {
    self->counters[self->num] = new_counter(slice_start, slice_end);
    self->num++;
}

// Transform RadixMap to Counter array.
// It will destory the map itself.
void write_to_file(CounterList *self, uchar *path) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("Can't write file: ");
        exit(errno);
    }

    for (size_t i = 0; i < self->num; i++) {
        fprintf(fp, "%llu\t%s\n", self->counters[i]->count, self->counters[i]->content);
        free_counter(self->counters[i]);
    }

    free(self);
    fclose(fp);
}

inline Counter *new_counter(uchar *slice_start, uchar *slice_end) {
    Counter *counter = (Counter *)malloc(sizeof(Counter));
    counter->content = slice_to_string(slice_start, slice_end);
    counter->count = 1;
    return counter;
}

inline void free_counter(Counter *self) {
    free(self->content);
    free(self);
}

uchar *slice_to_string(uchar *start, uchar *end) {
    uchar *str = (uchar *)malloc(sizeof(uchar) * (end - start + 1));
    uchar *to_ptr = str;
    uchar *from_ptr;
    for(from_ptr = start; from_ptr < end; from_ptr++) {
        *to_ptr = *from_ptr;
        to_ptr++;
    }
    *to_ptr = '\0';
    return str;
}

void sort_and_merge(CounterList *self) {
    qsort(self->counters, self->num, sizeof(Counter *), compare);

    size_t current = 0;
    for (size_t i = 1; i < self->num; i++) {
        if (strcmp(self->counters[current]->content, self->counters[i]->content) == 0) {
            self->counters[current]->count++;
            free_counter(self->counters[i]);
        } else {
            self->counters[current + 1] = self->counters[i];
            current++;
        }
    }
    self->num = current + 1;
}

inline int compare(const void *a, const void *b) {
    Counter *c = *(Counter **)a;
    Counter *d = *(Counter **)b;
    return strcmp(c->content, d->content);
}