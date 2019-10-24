#include "lib/option/options.h"
#include "lib/sort/insternal_sort/internal_sorter.h"
#include "lib/sort/external_sort/external_sorter.h"
#include <sys/time.h>

void print_duration(struct timeval start, struct timeval end);

int main(int argc, char *argv[]) {
    uchar *options[OPTIONS_NUM];
    setup_options(options, argc, (uchar **)argv);

    struct timeval start, end;

    gettimeofday(&start, NULL);
    InternalSorter *isorter = new_internal_sorter(options);
    segment(isorter);
    gettimeofday(&end, NULL);
    print_duration(start, end);

    gettimeofday(&start, NULL);
    ExternalSorter *esorter = new_external_sorter(isorter);
    k_way_merge(esorter);
    free_external_sorter(esorter);
    gettimeofday(&end, NULL);
    print_duration(start, end);
    return 0;
}

void print_duration(struct timeval start, struct timeval end) {
    printf("%lf\n", (double)(1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)) / 1000000);
}