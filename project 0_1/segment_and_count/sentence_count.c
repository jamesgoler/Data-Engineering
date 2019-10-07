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
    size_t radix = slice_start[0];
    //size_t radix = 0;

    // Insert as root.
    if (self->map[radix] == NULL) {
        self->map[radix] = new_tree_node(slice_start, slice_end);
        self->map[radix]->color = BLACK;
        self->usage++;
        return;
    }
    // try to find content in the tree.
    int clue;
    RedBlackTree *last, *ptr = self->map[radix];
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
    RedBlackTree *new = new_tree_node(slice_start, slice_end);
    new->parent = last;
    if (clue < 0) {
        last->left = new;
    } else {
        last->right = new;
    }
    red_black_adjust(&self->map[radix], new);
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
