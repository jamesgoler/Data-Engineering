#include "output_format.h"

void write_unit_as_tmp_lexical(FILE *fp, SortUnit *unit) {
    int count = 0;
    uchar *ptr;
    for (ptr = unit->key.value.lexical; *ptr != '\0'; ptr++) {
        if (*ptr == '\n') {
            count++;
        }
    }
    if (*(ptr - 1) != '\n') {
        size_t length = ptr - unit->key.value.lexical;
        unit->key.value.lexical = realloc(unit->key.value.lexical, length + 2);
        ptr = unit->key.value.lexical + length;
        *ptr = '\n';
        *(ptr + 1) = '\0';
        count++;
    }
    fprintf(fp, "%ld,%ld,%ld\n%s", unit->start, unit->length, count, unit->key.value.lexical);
    free(unit->key.value.lexical);
}

void write_unit_as_tmp_size(FILE *fp, SortUnit *unit) {
    fprintf(fp, "%ld,%ld,1\n%ld\n", unit->start, unit->length, unit->key.value.size);
}

void write_unit_as_tmp_numerical(FILE *fp, SortUnit *unit) {
    fprintf(fp, "%ld,%ld,1\n%lf\n", unit->start, unit->length, unit->key.value.numerical);
}