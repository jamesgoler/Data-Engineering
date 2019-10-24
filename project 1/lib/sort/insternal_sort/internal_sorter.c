#include "internal_sorter.h"
#include "functions/locate_record.h"
#include "functions/extract_key.h"
#include "functions/compare.h"
#include "functions/output_format.h"

extern int errno;

// Setup sorter with given options.
inline InternalSorter *new_internal_sorter(uchar **options) {
    InternalSorter *sorter = (InternalSorter *)malloc(sizeof(InternalSorter));

    sorter->buffer = (uchar *)malloc(sizeof(uchar) * BUFFER_SIZE);
    
    sorter->filename = options[path];

    sorter->input = open_data(options[path]);

    sorter->batch_num = 0;
    
    sorter->unit_num = 0;

    sorter->unique = (options[unique] != NULL);

    sorter->field_separator = options[field_separator];

    sorter->begin_pattern = options[begin];
    
    sorter->end_pattern = options[end];

    if (options[begin] && options[end]) {
        sorter->locate_record = locate_record;
    } else if (options[begin] && !options[end]) {
        sorter->locate_record = locate_record_only_begin;
    } else if (!options[begin] && options[end]) {
        sorter->locate_record = locate_record_only_end;
    } else {
        sorter->locate_record = locate_line;
    }

    if (options[key]) {
        sorter->key_pattern = options[key];
        sorter->key_pattern_length = strlen(options[key]);
    } else if (options[time_order]) {
        sorter->key_pattern = options[time_order];
        sorter->key_pattern_length = strlen(options[time_order]);
    } else {
        sorter->key_pattern = NULL;
        sorter->key_pattern_length = 0;
    }

    if (options[as_numerical]) {
        sorter->key_type = numerical;
        sorter->extract_key = extract_key_numerical;
        sorter->compare = options[reverse] ? compare_numerical_rev : compare_numerical;
        sorter->write_unit_as_tmp = write_unit_as_tmp_numerical;
    } else if (options[case_insensitive]) {
        sorter->key_type = lexical;
        sorter->extract_key = extract_key_case_insensitive;
        sorter->compare = options[reverse] ? compare_lexical_rev : compare_lexical;
        sorter->write_unit_as_tmp = write_unit_as_tmp_lexical;
    } else if (options[size_order]) {
        sorter->key_type = size;
        sorter->extract_key = extract_key_size_order;
        sorter->compare = options[reverse] ? compare_size_rev : compare_size;
        sorter->write_unit_as_tmp = write_unit_as_tmp_size;
    } else {
        sorter->key_type = lexical;
        sorter->extract_key = extract_key;
        sorter->compare = options[reverse] ? compare_lexical_rev : compare_lexical;
        sorter->write_unit_as_tmp = write_unit_as_tmp_lexical;
    }
    ensure_tmp_dir_exist();
    return sorter;
}

inline void free_internal_sorter(InternalSorter *self) {
    fclose(self->input);
    free(self);
}

void segment(InternalSorter *self) {
    uchar *record_end;
    long last_pos = 0;
    while ((record_end = self->locate_record(self))) {
        if (!self->found_key) {
            self->found_key = record_end;
            // fprintf(stderr, "%s, %ld", self->buffer, last_pos);
        }
        insert_unit(self, last_pos, record_end);
        last_pos = ftell(self->input);
    }

    if (self->unit_num > 0) {
        output_batch(self);
    }
}

inline void insert_unit(InternalSorter *self, long begin_pos, uchar *record_end) {
    self->units[self->unit_num].start = begin_pos;
    self->units[self->unit_num].length = (record_end - self->buffer);

    // Locate key.
    uchar *key_end;
    if (self->field_separator) {
        key_end = locate_field(self->found_key, self->field_separator);
    } else {
        if (self->key_pattern) {
            key_end = locate_newline(self->found_key);
        } else {
            key_end = record_end;
        }
    }
    if (!key_end) {
        key_end = record_end;
    }
    // Extract key. 
    self->units[self->unit_num].key = self->extract_key(self->found_key, key_end);

    self->unit_num++;
    if (self->unit_num == BATCH_SIZE) {
        output_batch(self);
    }
}

inline void output_batch(InternalSorter *self) {
    qsort(self->units, self->unit_num, sizeof(SortUnit), self->compare);
    if (self->unique) {
        merge(self);
    }

    char filename[24];
    sprintf(filename, "./temp/%ld", self->batch_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Can't generate temp files: ");
        exit(errno);
    }

    for (size_t i = 0; i < self->unit_num; i++) {
        self->write_unit_as_tmp(fp, &(self->units[i]));
    }
    fclose(fp);

    self->batch_num += 1;
    self->unit_num = 0;
}

inline void merge(InternalSorter *self) {
    size_t current = 0;
    for (size_t i = 1; i < self->unit_num; i++) {
        if (self->compare(&self->units[current], &self->units[i]) == 0) {
            if (self->key_type == lexical) {
                free(self->units[i].key.value.lexical);
            }
        } else {
            self->units[current + 1] = self->units[i];
            current++;
        }
    }
    self->unit_num = current + 1;
}

inline uchar *locate_field(uchar *line, uchar *separator) {
    return strstr(line, separator);
}

inline uchar *locate_newline(uchar *line) {
    while (*line != '\0') {
        if (*line == '\n') {
            return line + 1;
        }
        line++;
    }
    return line;
}

// Open data, exit when any error occurs.
inline FILE *open_data(uchar *path) {
    FILE *fp = fopen(path, "r");
    if(!fp) {
        perror("Can't read file: ");
        exit(errno);
    }
    return fp;
}

// Open directory, exit when any error occurs.
inline void ensure_tmp_dir_exist(void) {
    if (mkdir("./temp/", 0777) == -1) {
        if (errno != EEXIST) {
            perror("Can't generate temp dir: ");
            exit(errno);
        }
    }
}