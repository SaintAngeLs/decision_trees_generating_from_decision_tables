#include "xml_parser.h"
#include "char_list.h"
#include "parser_utils.h"
#include "string_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xalloc.h"

typedef struct XmlReadStates_ {
  enum XmlState {
    XSDefault,
    XSReadName,
    XSReadEnding,
    XSReadNewName,

    /* tag scanning */
    XSScanTag,
    XSReadPropertyName,
    XSReadAfterPropName,
    XSReadPropContents,
    XSExitSingleTag,

    XSIgnore
  } state;

  char const *input;
  char const *tmpNameRead;

  /*xml property reading*/
  char const *propNamePtr;
  char const *propNameEnd;
  char const *propValuePtr;

  StringNode *stack;
  size_t stack_size;

  CharList char_list;

} XmlReadStates;

static int is_w_regex_symbol(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
         (c >= '0' && c <= '9');
}

static void free_all(XmlReadStates *states) {
  destroy_char_list(&states->char_list);
  free_string_stack(states->stack, 0);
}

static int xs_push(XmlReadStates *xs) {
    size_t name_sz = xs->input - xs->tmpNameRead;

        char *new_name = xmalloc((name_sz + 1)*sizeof(char), __LINE__, __FILE__);
        if (!new_name) {
          return 0;
        }

        StringNode *new_node = xmalloc(sizeof(StringNode), __LINE__, __FILE__);
        if (!new_node) {
          free(new_name);
          return 0;
        }

        memset(new_node, 0, sizeof(*new_node));

        new_node->name = new_name;
        strncpy(new_name, xs->tmpNameRead, name_sz);
        new_name[name_sz] = 0;
        new_node->prev = xs->stack;
        xs->stack = new_node;
        ++xs->stack_size;
  return 1;
}

static int xs_pop(XmlReadStates *xs) {
  StringNode *parent = xs->stack->prev;
  StringTreeNode *childrenArray = NULL;
  char *xml_prop;

  if (xs->stack->nr_chilren) {
    childrenArray = xmalloc(sizeof(StringTreeNode) * xs->stack->nr_chilren, __LINE__, __FILE__);
    if (!childrenArray) {
      return 0;
    }
  }

    xml_prop = xmalloc((1+xs->char_list.size) * sizeof(char), __LINE__, __FILE__);
    if (!xml_prop) {
      free(childrenArray);
      return 0;
    }
    TreeListNode *newListNode = xmalloc(sizeof(TreeListNode), __LINE__, __FILE__);
    if (!newListNode) {
      free(childrenArray);
      free(xml_prop);
      return 0;
    }

  if (xs->stack->nr_chilren) {
    StringTreeNode* p = childrenArray;
    TreeListNode *tmp_child_node = xs->stack->childFirst;
    size_t i;

    for (i = xs->stack->nr_chilren; i; --i, ++p) {
      memcpy(p, &tmp_child_node->data, sizeof(StringTreeNode));
      tmp_child_node = tmp_child_node->next;
    }
  }

  if (1)
  {
    /*xml contents*/
    xml_prop[xs->char_list.size] = 0;

    /* add new child */
    if (parent->childLast) {
      parent->childLast->next = newListNode;
    } else {
      parent->childFirst = newListNode;
    }
    parent->childLast = newListNode;
    parent->nr_chilren++;
    newListNode->next = NULL;
    newListNode->data.name = xs->stack->name;
    xs->stack->name = NULL;
    if (xs->stack->nr_chilren) {
      newListNode->data.children = childrenArray;
      newListNode->data.nr_children = xs->stack->nr_chilren;
    } else {
      newListNode->data.children = NULL;
      newListNode->data.nr_children = 0;
    }
    free_tree_list(xs->stack->childFirst, 1);

    /* add the xml contents */
    strncpy(xml_prop, xs->char_list.arr, xs->char_list.size);
    newListNode->data.data = xml_prop;
    clear_char_list(&xs->char_list);
  }

  free(xs->stack);
  xs->stack = parent;
  --xs->stack_size;
  return 1;
}

StringTreeNode read_from_xml(const char *xml_contents) {
  StringTreeNode tree;
  memset(&tree, 0, sizeof(tree));

  XmlReadStates xs;
  xs.input = xml_contents;
  xs.state = XSDefault;
  xs.stack_size = 1;

  /* create the sentinel */
  xs.stack = xmalloc(sizeof(StringNode), __LINE__, __FILE__);
  if (!xs.stack) {
    return tree;
  }
  memset(xs.stack, 0, sizeof(*xs.stack));

  /* create char list for reading properties */
  if (!init_char_list(&xs.char_list)) {
    free(xs.stack);
    return tree;
  }

  /* parse */
  while (*xs.input != 0) {
    switch (xs.state) {
    case XSDefault:
      switch (*xs.input) {
      case '<':
        xs.state = XSReadName;
        break;
      default:
      {
        if (!add_to_char_list(*xs.input, &xs.char_list)) {
          free_all(&xs);
          return tree;
        }
        break;
      }
      }
      break;
    case XSReadName:
      switch (*xs.input) {
      case '/':
         if (xs.stack_size == 1) {
          free_all(&xs);
          return tree;
        }
        xs.tmpNameRead = xs.input + 1;
        xs.state = XSReadEnding;
        break;
      case '!':
      case '?':
        xs.state = XSIgnore;
        break;
      default:
        if (is_w_regex_symbol(*xs.input)) {
          xs.tmpNameRead = xs.input;
          xs.state = XSReadNewName;
        } else {
        }
        break;
      }
      break;
    case XSReadEnding:
      switch (*xs.input) {
      case '>':
        if (!xs_pop(&xs)) {
          free_all(&xs);
          return tree;
        }
        clear_char_list(&xs.char_list);
        xs.state = XSDefault;
        break;
      default:
        break;
      }
      break;
    case XSReadNewName:
      switch (*xs.input) {
      case '/':
        if (!xs_push(&xs)) {
          free_all(&xs);
          return tree;
        }
        xs.state = XSScanTag;
        --xs.input;
        break;
      case ' ':
      if (!xs_push(&xs)) {
          free_all(&xs);
          return tree;
        }
        /* <name [options] */
        xs.state = XSScanTag;
        break;
      case '>':
      {
        if (!xs_push(&xs)) {
          free_all(&xs);
          return tree;
        }
        clear_char_list(&xs.char_list);
        xs.state = XSDefault;
        break;
      }
      default:
        break;
      }
      break;
    case XSScanTag:
      switch (*xs.input) {
      case '/':
        if (!xs_pop(&xs)) {
          free_all(&xs);
          return tree;
        }
        /* <name [options] */
        xs.state = XSExitSingleTag;
        break;
      case '>':
      {
        clear_char_list(&xs.char_list);
        xs.state = XSDefault;
        break;
      }
      default:
        if (is_w_regex_symbol(*xs.input)) {
          xs.state = XSReadPropertyName;
          xs.propNamePtr = xs.input;
        } else {

        }
        break;
      }
      break;
    case XSReadPropertyName:
        switch (*xs.input) {
        case '=':
          xs.propNameEnd = xs.input;
          xs.state = XSReadAfterPropName;
          break;
        default:
          break;
        }
      break;
    case XSReadAfterPropName:
      switch (*xs.input) {
        case '\"':
          xs.propValuePtr = xs.input+1;
          xs.state = XSReadPropContents;
          break;
        default:
          /* add the prop */
          {
            TreeListNode *newListNode = xmalloc(sizeof(TreeListNode), __LINE__, __FILE__);
            if (!newListNode) {
              free_all(&xs);
              return tree;
            }
            char* newName = xmalloc(xs.propNameEnd - xs.propNamePtr + 1, __LINE__, __FILE__);
            if (!newName) {
              free(newListNode);
              free_all(&xs);
              return tree;
            }
            newListNode->data.data = NULL;
            newName[xs.propNameEnd - xs.propNamePtr] = 0;
            strncpy(newName, xs.propNamePtr, xs.propNameEnd - xs.propNamePtr);
            newListNode->data.children = NULL;
            newListNode->data.nr_children = 0;
            newListNode->data.name = newName;
            newListNode->next = NULL;
             if (xs.stack->childLast) {
              xs.stack->childLast->next = newListNode;
            } else {
              xs.stack->childFirst = newListNode;
            }
            xs.stack->childLast = newListNode;
            xs.stack->nr_chilren++;
          }
          xs.state = XSScanTag;
          --xs.input;
          break;
        }
      break;
    case XSReadPropContents:
      switch (*xs.input) {
        case '\"':
        {
          size_t value_sz = xs.input - xs.propValuePtr;
          char* newValue = NULL;
          if (value_sz) {
            newValue = xmalloc(xs.input - xs.propValuePtr + 1, __LINE__, __FILE__);
            if (!newValue) {
              free_all(&xs);
              return tree;
            }
            newValue[xs.input - xs.propValuePtr] = 0;
            strncpy(newValue, xs.propValuePtr, xs.input - xs.propValuePtr);
          }
            TreeListNode *newListNode = xmalloc(sizeof(TreeListNode), __LINE__, __FILE__);
            if (!newListNode) {
              free(newValue);
              free_all(&xs);
              return tree;
            }
            char* newName = xmalloc(xs.propNameEnd - xs.propNamePtr + 1, __LINE__, __FILE__);
            if (!newName) {
              free(newValue);
              free(newListNode);
              free_all(&xs);
              return tree;
            }
            newListNode->data.data = newValue;
            newName[xs.propNameEnd - xs.propNamePtr] = 0;
            strncpy(newName, xs.propNamePtr, xs.propNameEnd - xs.propNamePtr);
            newListNode->data.children = NULL;
            newListNode->data.nr_children = 0;
            newListNode->data.name = newName;
            newListNode->next = NULL;
             if (xs.stack->childLast) {
              xs.stack->childLast->next = newListNode;
            } else {
              xs.stack->childFirst = newListNode;
            }
            xs.stack->childLast = newListNode;
            xs.stack->nr_chilren++;
          
          xs.state = XSScanTag;
          break;
        }
        default:
          break;
        }
      break;
    case XSExitSingleTag:
    case XSIgnore:
      switch (*xs.input) {
      case '>':
      clear_char_list(&xs.char_list);
        xs.state = XSDefault;
        break;
      default:
        break;
      }  
      break;
    default:
      break;
    }
    ++xs.input;
  }

  if (xs.state != XSDefault || xs.stack_size != 1) {
    free_all(&xs);
    return tree;
  }

  if (xs.stack->nr_chilren) {
    /* create an array */
    StringTreeNode *chilrenArray =
        xmalloc(sizeof(StringTreeNode) * xs.stack->nr_chilren, __LINE__, __FILE__);

    if (!chilrenArray) {
      free_all(&xs);
      return tree;
    }

    TreeListNode *tmp_child_node = xs.stack->childFirst;
    StringTreeNode *p = chilrenArray;
    size_t i;
    for (i = xs.stack->nr_chilren; i; --i, ++p) {
      memcpy(p, &tmp_child_node->data, sizeof(StringTreeNode));
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
  destroy_char_list(&xs.char_list);
  return tree;
}