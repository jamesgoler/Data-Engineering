#include "external_sorter.h"
#include "../sort_unit.h"

extern int errno;

// Setup sorter with internal sorter, it'll eat it.
inline ExternalSorter *new_external_sorter(InternalSorter *isorter) {
    ExternalSorter *esorter = (ExternalSorter *)malloc(sizeof(ExternalSorter));
    
    esorter->key_type = isorter->key_type;

    esorter->filename = isorter->filename;
    
    esorter->input_offsets = open_temps(isorter->batch_num);

    esorter->tree_size = (isorter->batch_num << 1);

    esorter->batch_num = isorter->batch_num;

    esorter->unique = isorter->unique;

    esorter->buffer = isorter->buffer;

    esorter->compare = isorter->compare;

    esorter->extract_key = isorter->extract_key;

    esorter->free_unit =  free_unit_normal;

    esorter->units = (SortUnit **)malloc(sizeof(SortUnit *) * esorter->tree_size);
    for (size_t i = 1; i < esorter->tree_size; i++) {
        esorter->units[i] = (SortUnit *)malloc(sizeof(SortUnit));
    }
    fill_tree(esorter);

    free_internal_sorter(isorter);
    return esorter;
}

inline void free_external_sorter(ExternalSorter *self) {
    free(self->input_offsets);
    free(self->buffer);
    free(self->units);
    free(self);
}

inline size_t *open_temps(size_t file_num) {
    size_t *inputs = (size_t *)malloc(sizeof(size_t) * file_num);
    for (size_t i = 0; i < file_num; i++) {
        inputs[i] = 0;
    }
    return inputs;
}

void k_way_merge(ExternalSorter *self) {
    int input_fd = open(self->filename, O_RDONLY);
    int output_fd = open("result", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    size_t size;
    while (self->units[1]) {
        size = self->units[1]->length;
        
        lseek(input_fd, self->units[1]->start, SEEK_SET);
        
        if (size != read(input_fd, self->buffer, size)) {
            fprintf(stderr, "Error occurred in reading temp file.\n");
            exit(EXIT_FAILURE);
        }

        if (size != write(output_fd, self->buffer, size)) {
            fprintf(stderr, "Error occurred in writing file.\n");
            exit(EXIT_FAILURE);
        }
        
        if (self->key_type == lexical) {
            free(self->units[1]->key.value.lexical);
        }

        pull(self, 1);
    }
    close(input_fd);
    close(output_fd);
}

inline void fill_tree(ExternalSorter *self) {
    // supply leaves.
    for (size_t i = 0; i < self->batch_num; i++) {
        supply(self, i);
    }

    // size_t boundary = self->batch_num;
    // for (size_t i = self->batch_num >> 1; i > 0; i >>= 1) {
    //     for (size_t j = i; j < boundary; j++) {
    //         pull(self, j);
    //     }
    //     boundary = i;
    // }

    size_t start = self->batch_num;
    for (size_t i = self->batch_num >> 1; i > 0; i >>= 1) {
        for (size_t j = start - 1; j >= i; j--) {
            pull(self, j);
        }
        start = i;
    }
}

inline void pull(ExternalSorter *self, size_t unit_index) {
    SortUnit *tmp = self->units[unit_index];
    size_t current = unit_index;
    size_t children;
    int clue;
    while (current < self->batch_num) {
        children = current << 1;

        // Evaluate clue.
        if (self->units[children] && self->units[children + 1]) {
            clue = self->compare(self->units[children], self->units[children + 1]);
        } else if (!self->units[children] && self->units[children + 1]) {  
            clue = 1;
        } else if (self->units[children] && !self->units[children + 1]) {
            clue = -1;
        } else { // Current unit is a leaf.
            self->free_unit(tmp);
            self->units[current] = NULL;
            return;
        }
        
        // Pull units.
        if (clue > 0) { // left > right
            self->units[current] = self->units[children + 1];
            current = children + 1;
        } else if (clue < 0) {
            self->units[current] = self->units[children];
            current = children;
        } else {
            self->units[current] = self->units[children];
            current = children;
            if (self->unique) {
                pull(self, children + 1);
            }
        }
    }
    self->units[current] = tmp;
    supply(self, current - self->batch_num);
}

inline void supply(ExternalSorter *self, size_t input_index) {
    uchar filename[24];
    sprintf(filename, "./temp/%ld", input_index);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Opening temp files: ");
        exit(errno);
    }
    fseek(fp, self->input_offsets[input_index], SEEK_SET);

    size_t unit_index = input_index + self->batch_num;
    size_t start, length, line_num;
    if (fgets(self->buffer, BUFFER_SIZE, fp)) {
        parse(self->buffer, &start, &length, &line_num);

        // Input key to self->buffer.
        uchar *buffer = self->buffer;
        for (size_t i = 0; i < line_num; i++) {
            fgets(buffer, BUFFER_SIZE, fp);
            for (; *buffer != '\0'; buffer++);
        }
        Key key = self->extract_key(self->buffer, buffer);

        self->units[unit_index]->start = start;
        self->units[unit_index]->length = length;
        self->units[unit_index]->key = key;
        self->input_offsets[input_index] = ftell(fp);
    } else {
        self->free_unit(self->units[unit_index]);
        self->units[unit_index] = NULL;
    }
    fclose(fp);
}

// parse temp format to SortUnit;
inline void parse(uchar *line, size_t *start, size_t *length, size_t *line_num) {
    uchar *values[3];
    values[0] = line;
    values[1] = find_comma(values[0]) + 1;
    values[2] = find_comma(values[1]) + 1;

    *start = str_to_size_t(values[0]);
    *length = str_to_size_t(values[1]);
    *line_num = str_to_size_t(values[2]);
}

inline uchar *find_comma(uchar *str) {
    for (uchar *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == ',') {
            return ptr;
        }
    }
    fprintf(stderr, "Unexcepted file format.\n");
    exit(EXIT_FAILURE);
}

inline size_t str_to_size_t(uchar *str) {
    size_t value = 0;
    for (uchar *ptr = str; is_digit(*ptr); ptr++) {
        value = value * 10 + (*ptr - '0');
    }
    return value;
}

inline bool is_digit(uchar c) {
    return (c >= '0' && c <= '9');
}