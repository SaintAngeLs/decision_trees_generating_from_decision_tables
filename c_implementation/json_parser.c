#include "json_parser.h"
#include "parser_utils.h"
#include "string_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xalloc.h"

struct JsonReadStates {
  enum JssState { JSSDefault, JSSReadingString, JSSReadingQuoteless } state;

  const char *input;
  const char *tmpNameRead;

  char *name;
  char *str_input;

  StringNode *stack;
  size_t stack_size;
};

void free_all_json(struct JsonReadStates *states) {
  free(states->name);
  if (states->name != states->str_input)
    free(states->str_input);
  free_string_stack(states->stack, 0);
}

int json_add_property(struct JsonReadStates *jss) {
  TreeListNode *newChild = xmalloc(sizeof(TreeListNode), __LINE__, __FILE__);
  if (!newChild) {
    free_all_json(jss);
    return 0;
  }

  newChild->next = NULL;
  newChild->data.children = NULL;
  newChild->data.nr_children = 0;
  newChild->data.name = jss->name;
  jss->name = NULL;
  newChild->data.data = jss->str_input;
  jss->str_input = NULL;

  if (jss->stack->childLast) {
    jss->stack->childLast->next = newChild;
  } else {
    jss->stack->childFirst = newChild;
  }
  jss->stack->childLast = newChild;
  jss->stack->nr_chilren++;
  return 1;
}


StringTreeNode read_from_json(const char *json_contents) {
  StringTreeNode tree;
  memset(&tree, 0, sizeof(tree));

  struct JsonReadStates jss;
  jss.name = NULL;
  jss.str_input = NULL;

  jss.state = JSSDefault;
  jss.input = json_contents;
  jss.stack_size = 1;

  /* create the sentinel */
  jss.stack = xmalloc(sizeof(StringNode), __LINE__, __FILE__);
  if (!jss.stack) {
    return tree;
  }
  memset(jss.stack, 0, sizeof(*jss.stack));

  while (*jss.input) {
    switch (jss.state) {
    case JSSDefault: {
      switch (*jss.input) {
      case '\"':
        jss.state = JSSReadingString;
        jss.tmpNameRead = (jss.input + 1);
        break;
      case ':':
        /*add the name*/
        if (!jss.str_input || jss.name) {
          free_all_json(&jss);
          return tree;
        }
        jss.name = jss.str_input;
        jss.str_input = 0;
        break;
      case '{':
      case '[': {
        /*push*/
        StringNode *new_node = xmalloc(sizeof(StringNode), __LINE__, __FILE__);
        if (!new_node) {
          free_all_json(&jss);
          return tree;
        }
        memset(new_node, 0, sizeof(StringNode));
        new_node->prev = jss.stack;
        if (jss.name) {
          new_node->name = jss.name;
          jss.name = NULL;
        }
        jss.stack = new_node;
        ++jss.stack_size;
        break;
      }
      case '}':
      case ']': {
        /*pop*/
        if (jss.stack_size == 1) {
          free_all_json(&jss);
          return tree;
        }

        /*pop the str_input immediately*/
        if (jss.str_input) {
          /*add a property*/
          if (!json_add_property(&jss)) {
            return tree;
          }
        }

        StringTreeNode *chilrenArray = NULL;
        StringNode *parent = jss.stack->prev;

        if (jss.stack->nr_chilren) {
          /* create an array */
          chilrenArray = xmalloc(sizeof(StringTreeNode) * jss.stack->nr_chilren, __LINE__, __FILE__);

          if (!chilrenArray) {
            free_all_json(&jss);
            return tree;
          }

          TreeListNode *tmp_child_node = jss.stack->childFirst;
          for (size_t i = 0; i < jss.stack->nr_chilren; ++i) {
            memcpy(chilrenArray + i, &tmp_child_node->data,
                   sizeof(StringTreeNode));
            tmp_child_node = tmp_child_node->next;
          }
        }

        {
          TreeListNode *newListNode = xmalloc(sizeof(TreeListNode), __LINE__, __FILE__);
          if (!newListNode) {
            free(chilrenArray);
            free_all_json(&jss);
            return tree;
          }

          /*json contents*/

          if (parent->childLast) {
            parent->childLast->next = newListNode;
          } else {
            parent->childFirst = newListNode;
          }
          parent->childLast = newListNode;
          parent->nr_chilren++;
          newListNode->next = NULL;
          newListNode->data.name = jss.stack->name;
          jss.stack->name = NULL;

          if (jss.stack->nr_chilren) {
            newListNode->data.children = chilrenArray;
            newListNode->data.nr_children = jss.stack->nr_chilren;
          } else {
            newListNode->data.children = NULL;
            newListNode->data.nr_children = 0;
          }

          free_tree_list(jss.stack->childFirst, 1);

          newListNode->data.data = NULL;
        }

        free(jss.stack);
        jss.stack = parent;
        --jss.stack_size;
      } break;
      case ',':
      case ' ':
      case '\t':
      case '\n':
      case '\0':
        break;
      default:
        /*quoteless*/
        jss.state = JSSReadingQuoteless;
        jss.tmpNameRead = (jss.input + 0);
        break;
      }
      break;
    }
    case JSSReadingString: {
      if (*jss.input == '\"') {
        size_t name_sz = jss.input - jss.tmpNameRead;
        if (jss.str_input) {
          /*add a property*/
          if (!json_add_property(&jss)) {
            return tree;
          }
        }
        jss.str_input = xmalloc(name_sz + 1, __LINE__, __FILE__);
        if (!jss.str_input) {
          free_all_json(&jss);
          return tree;
        }
        memcpy(jss.str_input, jss.tmpNameRead, name_sz);
        jss.str_input[name_sz] = 0;
        jss.state = JSSDefault;
      }
      break;
    }
    case JSSReadingQuoteless: {
      if (*jss.input == ' ' || *jss.input == ',' || *jss.input == '\n' ||
          *jss.input == '\t' || *jss.input == '\0') {
        size_t name_sz = jss.input - jss.tmpNameRead;
        if (jss.str_input) {
          /*add a property*/
          if (!json_add_property(&jss)) {
            return tree;
          }
        }
        jss.str_input = xmalloc(name_sz + 1, __LINE__, __FILE__);
        if (!jss.str_input) {
          free_all_json(&jss);
          return tree;
        }
        memcpy(jss.str_input, jss.tmpNameRead, name_sz);
        jss.str_input[name_sz] = 0;
        jss.state = JSSDefault;
      }
      break;
    }
    default: {
      break;
    }
    }
    ++jss.input;
  }

  if (jss.stack_size != 1) {
    free_all_json(&jss);
    return tree;
  }

  if (jss.stack->nr_chilren) {
    StringTreeNode *chilrenArray =
        xmalloc(sizeof(StringTreeNode) * jss.stack->nr_chilren, __LINE__, __FILE__);

    if (!chilrenArray) {
      free_all_json(&jss);
      return tree;
    }

    TreeListNode *tmp_child_node = jss.stack->childFirst;
    for (size_t i = 0; i < jss.stack->nr_chilren; ++i) {
      memcpy(chilrenArray + i, &tmp_child_node->data, sizeof(StringTreeNode));
      tmp_child_node = tmp_child_node->next;
    }

    tree.children = chilrenArray;
    tree.nr_children = jss.stack->nr_chilren;

  } else {

    tree.children = NULL;
    tree.nr_children = 0;
  }

  free(jss.name);
  free(jss.str_input);
  free_string_stack(jss.stack, 1);

  return tree;
}

/*

 printf("[%p]\n", tree.children);
    printf("[%lu]\n", tree.nr_children);




*/