#ifndef OUTPUT_FORMAT_H_
#define OUTPUT_FORMAT_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include "../../sort_unit.h"
    
    void write_unit_as_tmp_lexical(FILE *fp, SortUnit *unit);

    void write_unit_as_tmp_size(FILE *fp, SortUnit *unit);

    void write_unit_as_tmp_numerical(FILE *fp, SortUnit *unit);

#endif