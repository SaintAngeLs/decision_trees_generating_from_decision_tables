/* approved */
#include "parser_utils.h"
#include <stdlib.h>
#include "xalloc.h"

void printStringTree(StringTreeNode* st, int level) {
	if (st->name) {
		for (int i = 0; i < level; ++i) {
		printf("    ");
	}
		printf("Name [%s]\n", st->name);
	}
	if (st->data) {
		for (int i = 0; i < level; ++i) {
		printf("    ");
	}
		printf("Prop [%s]\n", st->data);
	}
	for (int i = 0; i < level; ++i) {
		printf("    ");
	}
	printf("Nr of children: [%lu]\n", st->nr_children);
	for (size_t i = 0; i < st->nr_children; ++i) {
		for (int i = 0; i < level; ++i) {
			printf("    ");
		}
		printf("Child %lu:\n", i);
		printStringTree(&st->children[i], level+1);
	}
}

char* get_file_contents(FILE* fp) {
  /* size */
    fseek(fp, 0, SEEK_END); 
    long size = ftell(fp);
    if (size == 0) {
        return NULL; /* file is empty */
    }
    fseek(fp, 0, SEEK_SET);

    void* fcontent = xmalloc(size + 1, __LINE__, __FILE__);

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

char* get_str_from_file(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) return NULL;
	char* str = get_file_contents(fp);
	fclose(fp);
	return str;
}

/* for parsers */
void free_tree_list(TreeListNode *head, int keep_children) {
  while (head) {
    TreeListNode *temp = head;
    head = head->next;
    if (!keep_children)
      free_string_tree_children(temp->data);
    free(temp);
  }
}

/* for parsers */
void free_string_stack(StringNode *head, int keep_children) {
  while (head) {
    StringNode *temp = head;
    head = head->prev;
    free_tree_list(temp->childFirst, keep_children);
    free(temp->name);
    free(temp);
  }
}