#include "locate_record.h"

// If run out of buffer space, it should realloc, currently, not implement.

uchar *locate_record(InternalSorter *self) {
    size_t remain_space = BUFFER_SIZE;
    uchar *buffer = self->buffer;

    self->found_key = (self->key_pattern) ? NULL : buffer;

    while (fgets(buffer, remain_space, self->input)) {
        if (match(self->begin_pattern, buffer)) {
            if (!self->found_key && match(self->key_pattern, buffer)) {
                self->found_key = buffer + self->key_pattern_length;
            }
            // find next record.
            move_to_tail(&buffer, &remain_space);
            while (fgets(buffer, remain_space, self->input)) {
                if (!self->found_key && match(self->key_pattern, buffer)) {
                    self->found_key = buffer + self->key_pattern_length;
                }

                if (match(self->end_pattern, buffer)) {
                    move_to_tail(&buffer, &remain_space);
                    return buffer;
                }
                
                move_to_tail(&buffer, &remain_space);
            }
            fprintf(stderr, "Record end pattern not found!\n");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

uchar *locate_record_only_begin(InternalSorter *self) {
    size_t remain_space = BUFFER_SIZE;
    uchar *buffer = self->buffer;

    self->found_key = (self->key_pattern) ? NULL : buffer;

    while (fgets(buffer, remain_space, self->input)) {
        if (match(self->begin_pattern, buffer)) {
            if (!self->found_key && match(self->key_pattern, buffer)) {
                self->found_key = buffer + self->key_pattern_length;
            }
            // find next record.
            move_to_tail(&buffer, &remain_space);
            while (fgets(buffer, remain_space, self->input)) {
                if (!self->found_key && match(self->key_pattern, buffer)) {
                    self->found_key = buffer + self->key_pattern_length;
                }

                if (match(self->begin_pattern, buffer)) {
                    // Back to end of last record.
                    fseek(self->input, buffer - tail(buffer), SEEK_CUR);
                    return buffer;
                }
                
                move_to_tail(&buffer, &remain_space);
            }
            return buffer;
        }
    }
    return NULL;
}

uchar *locate_record_only_end(InternalSorter *self) {
    size_t remain_space = BUFFER_SIZE;
    uchar *buffer = self->buffer;

    self->found_key = (self->key_pattern) ? NULL : buffer;

    while (fgets(buffer, remain_space, self->input)) {
        if (!self->found_key && match(self->key_pattern, buffer)) {
            self->found_key = buffer + self->key_pattern_length;
        }

        if (match(self->end_pattern, buffer)) {
            move_to_tail(&buffer, &remain_space);
            return buffer;
        }
        
        move_to_tail(&buffer, &remain_space);
    }
    return NULL;
}

uchar *locate_line(InternalSorter *self) {
    size_t remain_space = BUFFER_SIZE;
    uchar *buffer = self->buffer;

    self->found_key = self->buffer;
    if (fgets(self->buffer, BUFFER_SIZE, self->input)) {
        move_to_tail(&buffer, &remain_space);
        return buffer;
    } else {
        return NULL;
    }
}

inline void move_to_tail(uchar **buffer, size_t *remain_space) {
    uchar *end = tail(*buffer);
    *remain_space -= (end - *buffer);
    if (*remain_space < 2) { // At least be able to contain a '\n' and a '\0'.
        fprintf(stderr, "Run out of buffer.\n");
        exit(EXIT_FAILURE);
    }
    *buffer = end;
}

// return the position of '\0'.
inline uchar *tail(uchar *line) {
    uchar *ptr;
    for (ptr = line; *ptr != '\0'; ptr++);
    return ptr;
}

inline bool match(uchar *a, uchar *b) {
    while (*a != '\0') {
        if (*a != *b) {
            return false;
        }
        a++;
        b++;
    }
    return true;
}