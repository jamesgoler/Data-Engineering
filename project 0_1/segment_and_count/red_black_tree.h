#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

#include "counter.h"

typedef enum Color{RED, BLACK} Color;

typedef struct RedBlackTree {
    Color color;
    Counter *counter;
    struct RedBlackTree *left;
    struct RedBlackTree *right;
    struct RedBlackTree *parent;
} RedBlackTree;

RedBlackTree *new_tree_node(uchar *slice_start, uchar *slice_end);

void free_tree_node(RedBlackTree *self);

void write_tree(FILE *fp, RedBlackTree *current);

void red_black_adjust(RedBlackTree **self, RedBlackTree *new_node);

void left_rotate(RedBlackTree **self, RedBlackTree *x);

void right_rotate(RedBlackTree **self, RedBlackTree *x);

int is_from_right(RedBlackTree *self);

int is_from_left(RedBlackTree *self);

int is_black(RedBlackTree *self);

int is_red(RedBlackTree *self);

RedBlackTree *grandparent(RedBlackTree *self);

RedBlackTree *uncle(RedBlackTree *self);

#endif