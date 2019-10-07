#ifndef SENTENCE_COUNT_H_
#define SENTENCE_COUNT_H_

#include <stddef.h>
#include <stdio.h>

#define MAP_SIZE 256

typedef struct Counter {
    char *content;
    size_t count;
} Counter;

typedef struct CounterTree {
    Counter *counter;
    struct CounterTree *left;
    struct CounterTree *right;
} CounterTree;

typedef struct RadixMap {
    CounterTree *map[MAP_SIZE];
    size_t usage;
    size_t sentence_count;
} RadixMap;

typedef unsigned char uchar;

RadixMap *new_radix_map(void);

void increase_or_insert(RadixMap *self, uchar *slice_start, uchar *slice_end);

void write_to_file(RadixMap *self, uchar *path);

CounterTree *new_tree_node(uchar *slice_start, uchar *slice_end);
void free_tree_node(CounterTree *self);

void write_tree(FILE *fp, CounterTree *current);

Counter *new_counter(uchar *slice_start, uchar *slice_end);
void free_counter(Counter *self);

uchar *slice_to_string(uchar *start, uchar *end);

int slice_cmp(uchar *start, uchar *end, uchar *str);

#endif