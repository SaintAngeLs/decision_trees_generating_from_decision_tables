#include "parser_utils.h"
#include <stdlib.h>

char* get_file_contents(FILE* fp) {
  /* size */
    fseek(fp, 0, SEEK_END); 
    long size = ftell(fp);
    if (size == 0) {
        return NULL; /* file is empty */
    }
    fseek(fp, 0, SEEK_SET);

    void* fcontent = malloc(size + 1);

    if (!fcontent) {
        return NULL;
    }

    if (size != fread(fcontent, 1, size, fp)) {
        free(fcontent);
        return NULL;
    }
    
    char* str = fcontent;
    str[size] = '\0';
    for (char* c = str+size-1; c+1 != str && (*c == '\n' || *c == '\r'); --c) {
        *c = '\0';
    }

    return str;
}

void free_tree_list(TreeListNode *head, int keep_children) {
  while (head) {
    TreeListNode *temp = head;
    head = head->next;
    if (!keep_children)
      free_string_tree_children(temp->data);
    free(temp);
  }
}

void free_string_stack(StringNode *head, int keep_children) {
  while (head) {
    StringNode *temp = head;
    head = head->prev;
    free_tree_list(temp->childFirst, keep_children);
    free(temp->name);
    free(temp);
  }
}