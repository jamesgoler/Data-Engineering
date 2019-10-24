#ifndef INTERNAL_SORTER_H_
#define INTERNAL_SORTER_H_

    #include <stdio.h>
    #include <string.h>
    #include <errno.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include "../../def.h"
    #include "../../option/options.h"
    #include "../sort_unit.h"

    #define BATCH_SIZE 131072
    #define BUFFER_SIZE 131072
    
    typedef struct InternalSorter {
        FILE *input;        // input source.
        size_t batch_num;   // current generated temp files.
        size_t unit_num;    // number of unit in current batch.

        bool unique;
        KeyType key_type;
        
        uchar *(*locate_record)(struct InternalSorter *); 
        Key (*extract_key)(uchar *, uchar *);
        int (*compare)(void *, void *);
        void (*write_unit_as_tmp)(FILE *, SortUnit *);

        uchar *filename;
        uchar *begin_pattern;
        uchar *end_pattern;
        uchar *field_separator;
        uchar *key_pattern;
        size_t key_pattern_length;
        uchar *found_key;

        uchar *buffer;
        SortUnit units[BATCH_SIZE];
    } InternalSorter;

    InternalSorter *new_internal_sorter(uchar **options);

    void free_internal_sorter(InternalSorter *self);

    void segment(InternalSorter *self);

    void insert_unit(InternalSorter *self, long begin_pos, uchar *record_end);

    void output_batch(InternalSorter *self);
    
    void merge(InternalSorter *self);

    uchar *locate_field(uchar *line, uchar *separator);

    uchar *locate_newline(uchar *line);

    FILE *open_data(uchar *path);

    void ensure_tmp_dir_exist(void);

#endif