#ifndef OPTIONS_H_
#define OPTIONS_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../def.h"

    #define OPTIONS_NUM 12

    typedef enum {
        path,
        begin, end,
        key, key2, time_order, size_order,
        as_numerical, case_insensitive,
        reverse, unique,
        field_separator
    } Option;

    void setup_options(uchar *options[], int argc, uchar *argv[]);

    void check_options(uchar **options);

    void parse_control_characters(uchar *str);

    void exit_with_error(char *message);

    void usage(void);

#endif