#ifndef COUNTER_H_
#define COUNTER_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct Counter {
    char *content;
    size_t count;
} Counter;

typedef unsigned char uchar;

Counter *new_counter(uchar *slice_start, uchar *slice_end);

void free_counter(Counter *self);

uchar *slice_to_string(uchar *start, uchar *end);

int slice_cmp(uchar *start, uchar *end, uchar *str);

#endif