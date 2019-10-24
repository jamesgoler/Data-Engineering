#ifndef LOCATE_RECORD_H_
#define LOCATE_REOCRD_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include "../../../def.h"
    #include "../internal_sorter.h"
    
    uchar *locate_record(InternalSorter *self);

    uchar *locate_record_only_begin(InternalSorter *self);

    uchar *locate_record_only_end(InternalSorter *self);

    uchar *locate_line(InternalSorter *self);

    void move_to_tail(uchar **buffer, size_t *remain_space);

    uchar *tail(uchar *line);

    bool match(uchar *a, uchar *b);

#endif