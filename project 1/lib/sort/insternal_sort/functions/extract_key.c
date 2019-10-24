#include "extract_key.h"

Key extract_key_size_order(uchar *start, uchar *end) {
    Key key;
    key.value.size = end - start;
    key.type = size;
    return key;
}

Key extract_key_numerical(uchar *start, uchar *end) {
    Key key;
    key.value.numerical = strtod(start, NULL);
    key.type = numerical;
    return key;
}

Key extract_key_case_insensitive(uchar *start, uchar *end) {
    Key key;
    key.value.lexical = slice_to_string(start, end);
    key.type = lexical;
    
    uchar *ptr;
    for (ptr = key.value.lexical; *ptr != '\0'; ptr++) {
        if (*ptr >= 'A' && *ptr <= 'Z') {
            *ptr -= 'A' - 'a';
        }
    }
    return key;
}

Key extract_key(uchar *start, uchar *end) {
    Key key;
    key.value.lexical = slice_to_string(start, end);
    key.type = lexical;
    return key;
}

inline uchar *slice_to_string(uchar *start, uchar *end) {
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