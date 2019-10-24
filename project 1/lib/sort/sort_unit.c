#include "sort_unit.h"

inline void free_unit_lexical(SortUnit *unit) {
    free(unit->key.value.lexical);
    free(unit);
}

inline void free_unit_normal(SortUnit *unit) {
    free(unit);
}
