#include <stdio.h>
#include <stdlib.h>

#include "red_black_tree.h"

inline RedBlackTree *new_tree_node(uchar *slice_start, uchar *slice_end) {
    RedBlackTree *node = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    node->color = RED;
    node->counter = new_counter(slice_start, slice_end);
    node->left = node->right = node->parent = NULL;
    return node;
}

inline void free_tree_node(RedBlackTree *self) {
    free_counter(self->counter);
    free(self);
}
// Move elements to list and free nodes. (Mix inorder and postorder traversal.)
void write_tree(FILE *fp, RedBlackTree *current) {
    if (current != NULL) {
        write_tree(fp, current->left);

        fprintf(fp, "%llu\t%s\n", current->counter->count, current->counter->content);

        write_tree(fp, current->right);

        free_tree_node(current);
    }
}

void red_black_adjust(RedBlackTree **self, RedBlackTree *new_node) {
    RedBlackTree *current = new_node;
    RedBlackTree *y;

    while (is_red(current->parent)) {
        y = uncle(current);
        if (is_red(y)) {
            y->color = BLACK;
            current->parent->color = BLACK;
            current = grandparent(current);
            current->color = RED;
        } else {
            if (is_from_left(current->parent)) {
                if (is_from_right(current)) {
                    current = current->parent;
                    left_rotate(self, current);
                }
                current->parent->color = BLACK;
                grandparent(current)->color = RED;
                right_rotate(self, grandparent(current));
            } else {
                if (is_from_left(current)) {
                    current = current->parent;
                    right_rotate(self, current);
                }
                current->parent->color = BLACK;
                grandparent(current)->color = RED;
                left_rotate(self, grandparent(current));
            }
        }
    }
    (*self)->color = BLACK;
}

inline void left_rotate(RedBlackTree **self, RedBlackTree *x) {
    RedBlackTree *y = x->right;

    // Transfer y->left to x->right.
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }

    // Transfer x to y->left.
    y->parent = x->parent;
    if (x->parent == NULL) { // x is root.
        (*self) = y;
    } else {
        if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
}

inline void right_rotate(RedBlackTree **self, RedBlackTree *x) {
    RedBlackTree *y = x->left;

    // Transfer y->right to x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }

    // Transfer x to y->right.
    y->parent = x->parent;
    if (x->parent == NULL) { // x is root
        (*self) = y;
    } else {
        if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    y->right = x;
    x->parent = y;
}

inline int is_from_right(RedBlackTree *self) {
    return (self == self->parent->right);
}

inline int is_from_left(RedBlackTree *self) {
    return (self == self->parent->left);
}

inline int is_black(RedBlackTree *self) {
    return (self == NULL || self->color == BLACK);
}

inline int is_red(RedBlackTree *self) {
    return (self != NULL && self->color == RED);
}

inline RedBlackTree *grandparent(RedBlackTree *self) {
    return (self->parent->parent);
}

inline RedBlackTree *uncle(RedBlackTree *self) {
    return (is_from_left(self) ? grandparent(self)->right : grandparent(self)->left);
}

