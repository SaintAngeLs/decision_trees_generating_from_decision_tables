#ifndef MINIML_TEXT_TREE_H
#define MINIML_TEXT_TREE_H
#include "unit_tests.h"
#include <stdint.h>
#include <stddef.h>

typedef struct _TextTreeNode {
    char* parent_text;
    char* node_text;
    
    struct _TextTreeNode* parent;
    struct _TextTreeNode* children;

    size_t nr_children;

    /* Hints */

    size_t lvl; /* 0 is root */
    size_t width; /* max range */

    size_t parent_index;
    
    struct _TextTreeNode* leaf_next;

    size_t left, right;

    /* Visualization hints */
    float x;
    float y;
    float text_x;
    float text_y;

    float ptr_text_x;
    float ptr_text_y;
    float ptr_x, ptr_y, ptr_px, ptr_py;

} TextTreeNode;

void print_decision_tree(TextTreeNode root, size_t depth);

void free_text_tree(TextTreeNode root);

#endif /* MINIML_TEXT_TREE_H */