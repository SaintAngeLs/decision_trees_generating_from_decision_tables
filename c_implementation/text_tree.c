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

void print_decision_tree(TextTreeNode root, size_t depth) {
  for (size_t i = 0; i < depth; ++i) {
    printf("    ");
  }
  printf("parent [%s] node [%s] nr_ch [%ld]\n", root.parent_text,
         root.node_text, root.nr_children);

  for (size_t i = 0; i < root.nr_children; ++i) {
    for (size_t i = 0; i < depth; ++i) {
      printf("    ");
    }
    printf("Child %ld:\n", i);
    print_decision_tree(root.children[i], depth + 1);
  }
}