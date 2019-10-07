#include "sentence_count.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern int errno;

inline RadixMap *new_radix_map(void) {
    RadixMap *map = (RadixMap *)malloc(sizeof(RadixMap));
    int i;
    for(i = 0; i < MAP_SIZE; i++) {
        map->map[i] = NULL;
    }
    map->usage = 0;
    map->sentence_count = 0;
    return map;
}

void increase_or_insert(RadixMap *self, uchar *slice_start, uchar *slice_end) {
    //size_t radix = slice_start[0];
    size_t radix = 0;
    // Insert as root.
    if (self->map[radix] == NULL) {
        self->map[radix] = new_tree_node(slice_start, slice_end);
        self->usage++;
        self->sentence_count++;
        return;
    }

    // try to find content in the tree.
    int clue;
    CounterTree *last, *ptr = self->map[radix];
    while (ptr != NULL) {
        last = ptr;
        clue = slice_cmp(slice_start, slice_end, ptr->counter->content);
        if (clue == 0) {
            ptr->counter->count++;
            return;
        } else if (clue < 0) {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
    }

    // new node.
    if (clue < 0) {
        last->left = new_tree_node(slice_start, slice_end);
    } else {
        last->right = new_tree_node(slice_start, slice_end);
    }
    self->sentence_count++;
}

// Transform RadixMap to Counter array.
// It will destory the map itself.
void write_to_file(RadixMap *self, uchar *path) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("Can't write file: ");
        exit(errno);
    }

    int i;
    for (i = 0; i < MAP_SIZE; i++) {
        if (self->map[i] != NULL) {
            write_tree(fp, self->map[i]);
        }
    }
    fclose(fp);
    free(self);
}

inline CounterTree *new_tree_node(uchar *slice_start, uchar *slice_end) {
    CounterTree *node = (CounterTree *)malloc(sizeof(CounterTree));
    node->counter = new_counter(slice_start, slice_end);
    node->left = node->right = NULL;
    return node;
}

inline void free_tree_node(CounterTree *self) {
    free_counter(self->counter);
    free(self);
}

// Move elements to list and free nodes. (Mix inorder and postorder traversal.)
void write_tree(FILE *fp, CounterTree *current) {
    if (current != NULL) {
        write_tree(fp, current->left);

        fprintf(fp, "%llu\t%s\n", current->counter->count, current->counter->content);

        write_tree(fp, current->right);

        free_tree_node(current);
    }
}

inline Counter *new_counter(uchar *slice_start, uchar *slice_end) {
    Counter *counter = (Counter *)malloc(sizeof(Counter));
    counter->content = slice_to_string(slice_start, slice_end);
    counter->count = 1;
    return counter;
}

inline void free_counter(Counter *self) {
    free(self->content);
    free(self);
}

uchar *slice_to_string(uchar *start, uchar *end) {
    uchar *str = (uchar *)malloc(sizeof(uchar) * (end - start + 1));
    uchar *to_ptr = str;
    uchar *from_ptr;
    for(from_ptr = start; from_ptr < end; from_ptr++) {
        *to_ptr = *from_ptr;
        to_ptr++;
    }
    *to_ptr = '\0';
    return str;
}

int slice_cmp(uchar *start, uchar *end, uchar *str) {
    while (start < end) {
        if (*start == *str) {
            start++;
            str++;
        } else {
            return (*start - *str);
        }
    }
    return 0;
}