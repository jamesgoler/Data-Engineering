#ifndef COMPARE_H_
#define COMPARE_H_

    #include <string.h>
    #include "../../sort_unit.h"

    typedef int (*Compare)(void *, void *);

    int compare_numerical(void *a, void *b);

    int compare_lexical(void *a, void *b);

    int compare_size(void *a, void *b);

    int compare_numerical_rev(void *a, void *b);

    int compare_lexical_rev(void *a, void *b);

    int compare_size_rev(void *a, void *b);

#endif