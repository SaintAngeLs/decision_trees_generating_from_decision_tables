#include "xml_parser.h"
#include "parser_utils.h"
#include "string_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct XmlReadStates {
  enum XmlState {
    XSDefault,
    XSReadName,
    XSReadEnding,
    XSReadNewName,
  } state;

  const char *input;
  const char *tmpNameRead;

  StringNode *stack;
  size_t stack_size;

  struct CharNode *char_list;
  struct CharNode *char_list_null;
  size_t char_list_size_with_term;
};

void free_all(struct XmlReadStates *states) {
  free_char_list(states->char_list_null);
  free_string_stack(states->stack, 0);
}

StringTreeNode read_from_xml(const char *xml_contents) {
  StringTreeNode tree;
  memset(&tree, 0, sizeof(tree));

  struct XmlReadStates xs;

  xs.input = xml_contents;
  xs.state = XSDefault;

  xs.stack_size = 1;

  /* create the sentinel */
  xs.stack = malloc(sizeof(StringNode));
  if (!xs.stack) {

    return tree;
  }
  memset(xs.stack, 0, sizeof(*xs.stack));

  /* create char list for reading properties */
  xs.char_list = malloc(sizeof(struct CharNode));
  if (!xs.char_list) {
    free(xs.stack);
    return tree;
  }
  memset(xs.char_list, 0, sizeof(struct CharNode));

  xs.char_list_null = xs.char_list;
  xs.char_list_size_with_term = 1;

  /* parse */
  while (*xs.input != 0) {

    /* Reading properties */
    if (xs.state == XSDefault) {
      if (*xs.input == '<') {
        xs.state = XSReadName;
      } else {
        struct CharNode *newCharNode = malloc(sizeof(struct CharNode));
        if (!newCharNode) {
          free_all(&xs);
          return tree;
        }
        newCharNode->data = *xs.input;
        {
          struct CharNode *next_elem = xs.char_list->prev;
          (newCharNode->next) = xs.char_list;
          (newCharNode->prev) = next_elem;
          (xs.char_list->prev) = newCharNode;
          if (next_elem)
            (next_elem->next) = newCharNode;
        }
        xs.char_list = newCharNode;
        ++xs.char_list_size_with_term;
      }

      /* Reading name */
    } else if (xs.state == XSReadName) {
      if (*xs.input == '/') {

        if (xs.stack_size == 1) {
          free_all(&xs);
          return tree;
        }

        xs.tmpNameRead = xs.input + 1;
        xs.state = XSReadEnding;
      } else {
        xs.tmpNameRead = xs.input;
        xs.state = XSReadNewName;
      }

      /* Reading new name */
    } else if (xs.state == XSReadNewName) {
      if (*xs.input == '>') {

        size_t name_sz = xs.input - xs.tmpNameRead;
        char *new_name = malloc(name_sz + 1);
        if (!new_name) {
          free_all(&xs);
          return tree;
        }
        StringNode *new_node = malloc(sizeof(StringNode));
        if (!new_node) {
          free(new_name);
          free_all(&xs);
          return tree;
        }
        memset(new_node, 0, sizeof(*new_node));
        new_node->data = new_name;
        strncpy(new_name, xs.tmpNameRead, xs.input - xs.tmpNameRead);
        new_name[name_sz] = 0;
        new_node->prev = xs.stack;
        xs.stack = new_node;
        ++xs.stack_size;

        xs.state = XSDefault;
      } else {
        /* scan the title */
      }

      /* Reading /name */
    } else if (xs.state == XSReadEnding) {
      if (*xs.input == '>') {

        size_t name_sz = xs.input - xs.tmpNameRead;
        int ctmp = strncmp(xs.stack->data, xs.tmpNameRead, name_sz);

        if (ctmp != 0) {
          free_all(&xs);
          return tree;
        }

        StringTreeNode *chilrenArray = NULL;
        StringNode *parent = xs.stack->prev;

        if (xs.stack->nr_chilren) {
          /* create an array */
          chilrenArray = malloc(sizeof(StringTreeNode) * xs.stack->nr_chilren);

          if (!chilrenArray) {
            free_all(&xs);
            return tree;
          }

          TreeListNode *tmp_child_node = xs.stack->childFirst;
          for (size_t i = 0; i < xs.stack->nr_chilren; ++i) {
            memcpy(chilrenArray + i, &tmp_child_node->data,
                   sizeof(StringTreeNode));
            tmp_child_node = tmp_child_node->next;
          }
        }

        {
          TreeListNode *newListNode = malloc(sizeof(TreeListNode));
          if (!newListNode) {
            free(chilrenArray);
            free_all(&xs);
            return tree;
          }

          /*xml contents*/

          char *xml_prop = malloc(xs.char_list_size_with_term * sizeof(char));
          if (!xml_prop) {
            free(newListNode);
            free(chilrenArray);
            free_all(&xs);
            return tree;
          }

          if (parent->childLast) {
            parent->childLast->next = newListNode;
          } else {
            parent->childFirst = newListNode;
          }
          parent->childLast = newListNode;
          parent->nr_chilren++;
          newListNode->next = NULL;
          newListNode->data.name = xs.stack->data;
          xs.stack->data = NULL;

          if (xs.stack->nr_chilren) {
            newListNode->data.children = chilrenArray;
            newListNode->data.nr_children = xs.stack->nr_chilren;
          } else {
            newListNode->data.children = NULL;
            newListNode->data.nr_children = 0;
          }

          free_tree_list(xs.stack->childFirst, 1);

          struct CharNode *tmp_char_node = xs.char_list_null->prev;
          for (size_t i = 0; i < xs.char_list_size_with_term - 1; ++i) {
            xml_prop[i] = tmp_char_node->data;
            tmp_char_node = tmp_char_node->prev;
          }
          xml_prop[xs.char_list_size_with_term - 1] = 0;

          newListNode->data.data = xml_prop;

          xs.char_list = xs.char_list_null;
          tmp_char_node = xs.char_list_null->prev;

          /* new char list */
          while (tmp_char_node) {
            struct CharNode *tmp_cnp = tmp_char_node;
            tmp_char_node = tmp_char_node->prev;
            free(tmp_cnp);
          }

          xs.char_list->next = NULL;
          xs.char_list->prev = NULL;
          xs.char_list_size_with_term = 1;
        }

        free(xs.stack);
        xs.stack = parent;
        --xs.stack_size;

        xs.state = XSDefault;
      } else {
        /* scan the title */
      }
    }
    ++xs.input;
  }

  /* wrong xml ending */
  if (xs.state != XSDefault || xs.stack_size != 1) {
    free_all(&xs);
    return tree;
  }

  if (xs.stack->nr_chilren) {
    /* create an array */
    StringTreeNode *chilrenArray =
        malloc(sizeof(StringTreeNode) * xs.stack->nr_chilren);

    if (!chilrenArray) {
      free_all(&xs);
      return tree;
    }

    TreeListNode *tmp_child_node = xs.stack->childFirst;
    for (size_t i = 0; i < xs.stack->nr_chilren; ++i) {
      memcpy(chilrenArray + i, &tmp_child_node->data, sizeof(StringTreeNode));
      tmp_child_node = tmp_child_node->next;
    }

    free_tree_list(xs.stack->childFirst, 1);

    tree.children = chilrenArray;
    tree.nr_children = xs.stack->nr_chilren;

  } else {

    tree.children = NULL;
    tree.nr_children = 0;
  }

  free(xs.stack);
  free_char_list(xs.char_list_null);
  return tree;
}