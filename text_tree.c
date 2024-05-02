#include "text_tree.h"
#include <stdlib.h>

void free_text_tree(TextTreeNode root) {
    free(root.parent_text);
    free(root.node_text);
    for (size_t i = 0; i < root.nr_children; ++i) {
        free_text_tree(root.children[i]);
    }
    free(root.children);
}