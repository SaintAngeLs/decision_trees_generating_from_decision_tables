#ifndef MINIML_CHAR_LIST_H
#define MINIML_CHAR_LIST_H
#include <stddef.h>
#include <stdint.h>

typedef struct CharList_ {
  char *arr;
  size_t size;
  size_t capacity;
} CharList;

int init_char_list(CharList* list);
int add_to_char_list(char c, CharList* list);
void clear_char_list(CharList* list);
void destroy_char_list(CharList* list);

#endif /* MINIML_CHAR_LIST_H */