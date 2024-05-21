#include "string_tree.h"
#include <stdlib.h>

void free_string_tree_children(StringTreeNode tree_root) {
  free(tree_root.data);
  free(tree_root.name);
  if (tree_root.children != NULL) {
    for (size_t i = 0; i < tree_root.nr_children; ++i) {
      free_string_tree_children(tree_root.children[i]);
    }
    free(tree_root.children);
  }
}