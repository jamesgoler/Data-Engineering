#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "sentence_count.h"
#include "sentence_segmentation.h"

#define BUFFER_SIZE 131072

extern int errno;

FILE *open_data(uchar *path);

int main() {
    uchar buffer[BUFFER_SIZE];
    uchar *start, *end;

    FILE *fp = open_data("../data/ettoday.rec");

    RadixMap *map = new_radix_map();

    clock_t duration_build, duration_output;
    clock_t cstart = clock();

    while (fgets(buffer, BUFFER_SIZE, fp)) {
        if (is_body_block(buffer)) {
            // Jump over "@body:"
            start = buffer + 6;
            
            // Handle lines of current body block.
            do {
                remove_control_chars(buffer);
                // Locate sentences and count them.
                while ((end = locate_sentence(&start))) {
                    increase_or_insert(map, start, end);
                    start = end;
                }
                start = buffer;
            } while(fgets(buffer, BUFFER_SIZE, fp) && still_in_body_block(buffer));
        }
    }
    fclose(fp);
    clock_t cend = clock();
    duration_build = cend - cstart;

    cstart = clock();
    write_to_file(map, "result.txt");
    cend = clock();
    
    duration_output = cend - cstart;
    printf("%lf, %lf\n", (double)(duration_build) / CLOCKS_PER_SEC, (double)(duration_output) / CLOCKS_PER_SEC);
    return 0;
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
