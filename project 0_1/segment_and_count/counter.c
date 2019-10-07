#include "counter.h"

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

int slice_cmp(uchar *start, uchar *end, uchar *str) {
    while (start < end) {
        if (*start == *str) {
            start++;
            str++;
        } else {
            return (*start - *str);
        }
    }
    return 0;
}