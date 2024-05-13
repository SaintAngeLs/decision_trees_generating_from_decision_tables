#ifndef MINIML_PARSER_UTILS_H
#define MINIML_PARSER_UTILS_H
#include "string_tree.h"
#include <stdio.h>

/* used in stack for temporary children list */
typedef struct TreeListNode_ {
  StringTreeNode data;
  struct TreeListNode_ *next;  
} TreeListNode;

/* stack node */
typedef struct StringNode_ {
  struct StringNode_ *prev;
  char *name; /* tag name */

  TreeListNode *childFirst, *childLast;
  size_t nr_chilren;
} StringNode;

void free_tree_list(TreeListNode *head, int keep_children);
void free_string_stack(StringNode *head, int keep_children);
char* get_file_contents(FILE* fp);

/* Get string from file with given filename
   Returns NULL if failed.
   Returns allocated char* which supposed to be freed after use.
 */
char* get_str_from_file(const char* filename);

void printStringTree(StringTreeNode* st, int level);

#endif /* MINIML_PARSER_UTILS_H */