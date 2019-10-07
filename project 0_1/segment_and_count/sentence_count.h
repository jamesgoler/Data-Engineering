#ifndef SENTENCE_COUNT_H_
#define SENTENCE_COUNT_H_

#include <stddef.h>
#include <stdio.h>

#include "red_black_tree.h"

#define MAP_SIZE 256

typedef struct RadixMap {
    RedBlackTree *map[MAP_SIZE];
    size_t usage;
    size_t sentence_count;
} RadixMap;

typedef unsigned char uchar;

RadixMap *new_radix_map(void);

void increase_or_insert(RadixMap *self, uchar *slice_start, uchar *slice_end);

void write_to_file(RadixMap *self, uchar *path);

#endif
