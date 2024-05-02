#ifndef MINIML_PARSER_UTILS_H
#define MINIML_PARSER_UTILS_H
#include "string_tree.h"
#include <stdio.h>

/* used in stack for temporary children list */
typedef struct _TreeListNode {
  struct _TreeListNode *next;
  StringTreeNode data;
} TreeListNode;

/* stack node */
typedef struct _StringNode {
  struct _StringNode *prev;
  char *data; /* tag */
  TreeListNode *childFirst, *childLast;
  size_t nr_chilren;
} StringNode;

struct CharNode {
  struct CharNode *next, *prev;
  char data;
};

void free_tree_list(TreeListNode *head, int keep_children);
void free_string_stack(StringNode *head, int keep_children);
void free_char_list(struct CharNode *head);

char* get_file_contents(FILE* fp);

#endif /* MINIML_PARSER_UTILS_H */