#ifndef SENTENCE_COUNT_H_
#define SENTENCE_COUNT_H_

#include <stddef.h>
#include <stdio.h>

#define LIST_SIZE 16777216

typedef struct Counter {
    char *content;
    size_t count;
} Counter;

typedef struct CounterList {
    Counter *counters[LIST_SIZE];
    size_t num;
} CounterList;

typedef unsigned char uchar;

CounterList *new_counter_list(void);

void insert(CounterList *self, uchar *slice_start, uchar *slice_end);

void write_to_file(CounterList *self, uchar *path);

Counter *new_counter(uchar *slice_start, uchar *slice_end);

void free_counter(Counter *self);

uchar *slice_to_string(uchar *start, uchar *end);

void sort_and_merge(CounterList *self);

int compare(const void *a, const void *b);

#endif