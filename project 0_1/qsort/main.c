#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "sentence_count.h"
#include "sentence_segmentation.h"

#define BUFFER_SIZE 131072

extern int errno;

FILE *open_data(uchar *path);
int compare(const void *a, const void *b);
size_t merge(Counter **counter, size_t num);

int main() {
    uchar buffer[BUFFER_SIZE];
    uchar *start, *end;

    FILE *fp = open_data("../data/ettoday.rec");

    CounterList *counter_list = new_counter_list();

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
                    insert(counter_list, start, end);
                    start = end;
                }
                start = buffer;
            } while(fgets(buffer, BUFFER_SIZE, fp) && still_in_body_block(buffer));
        }
    }

    sort_and_merge(counter_list);

    clock_t cend = clock();
    duration_build = cend - cstart;

    cstart = clock();
    write_to_file(counter_list, "result.txt");
    cend = clock();
    duration_output = cend - cstart;

    printf("%lf, %lf\n", (double)(duration_build) / CLOCKS_PER_SEC, (double)(duration_output) / CLOCKS_PER_SEC);
    fclose(fp);
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
