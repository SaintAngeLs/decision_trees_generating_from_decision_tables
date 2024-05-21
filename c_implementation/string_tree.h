#ifndef MINIML_STRING_TREE_H
#define MINIML_STRING_TREE_H
#include <stddef.h>
#include <stdint.h>

typedef struct _StringTreeNode {
  struct _StringTreeNode *children;
  char *data;
  char *name;
  size_t nr_children;
} StringTreeNode;

void free_string_tree_children(StringTreeNode tree_root);

#endif /* MINIML_STRING_TREE_H */