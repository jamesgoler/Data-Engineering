#ifndef SORT_UNIT_H_
#define SORT_UNIT_H_

    #include <stdlib.h>
    #include <stddef.h>
    #include "../def.h"

    typedef enum KeyType {
        size, numerical, lexical
    } KeyType;

    typedef union KeyValue {
        size_t size;
        double numerical;
        uchar *lexical;
    } KeyValue;

    typedef struct Key {
        KeyType type;
        KeyValue value;
    } Key;

    typedef struct SortUnit {
        size_t start;
        size_t length;
        Key key;
    } SortUnit;

    typedef void (*FreeUnit)(SortUnit *);

    void free_unit_lexical(SortUnit *unit);

    void free_unit_normal(SortUnit *unit);

#endif