#include "options.h"

static inline bool match(uchar *a, uchar *b) {
    return (strcmp(a, b) == 0);
}

// Change order of argv such that following process of building process maybe simplified.
void setup_options(uchar *options[], int argc, uchar *argv[]) {
    bool has_given_path = false;

    // Clear options.
    for (int i = 0; i < OPTIONS_NUM; i++) {
        options[i] = NULL;
    }

    // Enable options.
    uchar *arg;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            arg = argv[i] + 1;
            switch (*arg) { // I really hate it, but having no choice.
                case 'r':
                    if (match(arg + 1, "b") && ((i + 1) < argc)) {
                        parse_control_characters(argv[++i]);
                        options[begin] = argv[i];

                    } else if (match(arg + 1, "e") && ((i + 1) < argc)) {
                        parse_control_characters(argv[++i]);
                        options[end] = argv[i];

                    } else if (*(arg + 1) == '\0') {
                        options[reverse] = arg;

                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;

                case 'k':
                    if (*(arg + 1) == '\0' && ((i + 1) < argc)) {
                        parse_control_characters(argv[++i]);
                        options[key] = argv[i];

                    } else if (match(arg + 1, "2") && ((i + 1) < argc)) {
                        parse_control_characters(argv[++i]);
                        options[key2] = argv[i];

                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;
                
                case 't':
                    if (*(arg + 1) == '\0' && ((i + 1) < argc)) {
                        parse_control_characters(argv[++i]);
                        options[time_order] = argv[i];

                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;

                case 's':
                    if (*(arg + 1) == '\0') {
                        options[size_order] = argv[i];

                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;

                case 'i':
                    if (*(arg + 1) == '\0') {
                        options[case_insensitive] = argv[i];

                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;

                case 'n':
                    if (*(arg + 1) == '\0') {
                        options[as_numerical] = argv[i];
                    
                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;

                case 'u':
                    if (*(arg + 1) == '\0') {
                        options[unique] = argv[i];
                    
                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;

                case 'f':
                    if (*(arg + 1) == '\0' && ((i + 1) < argc)) {
                        parse_control_characters(argv[++i]);
                        options[field_separator] = argv[i];
                    
                    } else {
                        exit_with_error(argv[i]);
                    }
                    break;
                
                default:
                    exit_with_error(argv[i]);
            }
        } else {
            if (!has_given_path) {
                options[path] = argv[i];

            } else {
                exit_with_error(argv[i]);
            }
        }
    }
    check_options(options);
}

inline void check_options(uchar **options) {
    // Check input.
    if (!options[path]) {
        fprintf(stderr, "Missing input file.\n");
        exit(EXIT_FAILURE);
    }

    // Check record patterns.
    if (!options[begin] && !options[end]) {
        if (options[key] || options[key2] || options[time_order]) {
            fprintf(stderr, "Conflict argument: Line-based String with key.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Check key pattern.
    if (options[key2]) {
        fprintf(stderr, "Secondary key has not been implemented currently.\n");
        exit(EXIT_FAILURE);
    }
    if (options[key] && options[time_order]) {
        fprintf(stderr, "Multiple key has not been implemented currently.\n");
        exit(EXIT_FAILURE);
    }
    if (options[time_order] && options[as_numerical]) {
        fprintf(stderr, "Conflict argument: Time String can't transfrom to numerical(double).\n");
        exit(EXIT_FAILURE);
    }

    // Check key type
    if (options[case_insensitive] && options[as_numerical]) {
        fprintf(stderr, "Conflict argument: Numerical key with case-insensitive.\n");
        exit(EXIT_FAILURE);
    }
}

inline void parse_control_characters(uchar *str) {
    uchar *in, *out;
    in = out = str;
    while (*in != '\0') {
        if (*in == '\\') {
            switch (*(in + 1)) {
                case 'n':
                    *out = '\n';
                    in++;
                    break;

                case 't':
                    *out = '\t';
                    in++;
                    break;

                default:
                    *out = *in;
                    break;
            }
        } else {
            *out = *in;
        }
        out++;
        in++;
    }
    *out = '\0';
}

inline void exit_with_error(char *message) {
    fprintf(stderr, "Invalid argument: %s\n", message);
    usage();
    exit(EXIT_FAILURE);
}

inline void usage(void) {
    fprintf(stderr, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        "External Sort",
        "[Options]",
            "\t-rb PATTERN:\tDesignate what pattern records begin with.",
                "\t\t\tDefault will treat each line as a record, so if your records are line-based string, you needn't to have this option.",
            "\t-re PATTERN:\tDesignate what pattern records end with.",
                "\t\t\tYou may want to use this option if record begin pattern is not clear (so -rb is not presented here),",
                "\t\t\tor there are some unwanted contents between two records (which means that it will remove some original content.)",
            "\t-r         :\tSort in descending order. (If this option is not presented, default is ascending order.)",
            "\t-k  PATTERN:\tDesignate where keys appear.",
                "\t\t\tNote that keys will only be extracted as a line key unless -f is enabled.",
            "\t-k2 PATTERN:\tDesignate where secondary keys of record appear. Not implement yet.",
            "\t-t  PATTERN:\tUse time order.",
            "\t-s         :\tUse size order.",
            "\t-f  PATTERN:\tDesignate what pattern each field begin with, it will enable multi-line key at the same time.",
            "\t-i         :\tCase insensitive.",
            "\t-u         :\tUniquify the result, records has duplicated key will be remove from result."
    );
}