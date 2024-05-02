#ifndef MINIML_TEXT_TREE_H
#define MINIML_TEXT_TREE_H
#include "unit_tests.h"
#include <stdint.h>
#include <stddef.h>

typedef struct _TextTreeNode {
    uint_least32_t* parent_text;
    uint_least32_t* node_text;
    
    /* Visualization hints */
    size_t left, right;

    struct _TextTreeNode* parent;
    struct _TextTreeNode* children;
    size_t nr_children;

} TextTreeNode;

void free_text_tree(TextTreeNode root);

#endif /* MINIML_TEXT_TREE_H */