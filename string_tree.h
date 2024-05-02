#ifndef MINIML_STRING_TREE_H
#define MINIML_STRING_TREE_H
#include <stddef.h>

typedef struct _StringTreeNode {
    struct _StringTreeNode* children;
    size_t nr_children;
    char* data;
    char* name;
} StringTreeNode;

void free_string_tree_children(StringTreeNode tree_root);

#endif /* MINIML_STRING_TREE_H */