#ifndef EXTERNAL_SORTER_H_
#define EXTERNAL_SORTER_H_

    #include <stdio.h>
    #include <string.h>
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include "../../def.h"
    #include "../../option/options.h"
    #include "../sort_unit.h"
    #include "../insternal_sort/internal_sorter.h"

    typedef struct ExternalSorter {
        size_t *input_offsets;
        size_t batch_num;
        size_t tree_size;
        uchar *filename;
        uchar *buffer;
        SortUnit **units;

        bool unique;
        KeyType key_type;
        
        Key (*extract_key)(uchar *, uchar *);
        int (*compare)(void *, void *);
        FreeUnit free_unit;
    } ExternalSorter;

    ExternalSorter *new_external_sorter(InternalSorter *isorter);

    void free_external_sorter(ExternalSorter *self);

    size_t *open_temps(size_t file_num);

    void k_way_merge(ExternalSorter *self);

    void fill_tree(ExternalSorter *self);

    void pull(ExternalSorter *self, size_t unit_index);

    void supply(ExternalSorter *self, size_t input_index);

    void parse(uchar *line, size_t *start, size_t *end, size_t *line_num);

    uchar *find_comma(uchar *str);

    size_t str_to_size_t(uchar *str);

    bool is_digit(uchar c);

#endif