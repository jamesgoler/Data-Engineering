#ifndef KEY_EXTRACTION_H_
#define KEY_EXTRACTION_H_

    #include <stdlib.h>
    #include "../../sort_unit.h"
    #include "../../../def.h"

    typedef Key (*ExtractKey)(uchar *, uchar *);
    
    Key extract_key_size_order(uchar *start, uchar *end);

    Key extract_key_numerical(uchar *start, uchar *end);

    Key extract_key_case_insensitive(uchar *start, uchar *end);

    Key extract_key(uchar *start, uchar *end);

    uchar *slice_to_string(uchar *start, uchar *end);
    
#endif 