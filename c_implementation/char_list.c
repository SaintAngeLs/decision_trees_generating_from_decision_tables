#include "char_list.h"
#include <string.h>
#include <stdlib.h>
#include "xalloc.h"

#define CHUNK_SIZE (1024*4)

int init_char_list(CharList* list) {
    list->capacity = 0;
    list->size = 0;
    list->arr = xmalloc(CHUNK_SIZE, __LINE__, __FILE__);
    if (!list->arr) {
        return 0;
    }
    list->capacity = CHUNK_SIZE;
    list->size = 0;
    return 1;
}

int add_to_char_list(char c, CharList* list) {
    if (list->size >= list->capacity) {
        void* re = xrealloc(list->arr, list->capacity + CHUNK_SIZE, __LINE__, __FILE__);
        if (!re) {
            return 0;
        }
        list->arr = re;
        list->capacity += CHUNK_SIZE;
    }
    list->arr[list->size] = c;
    ++list->size;
    return 1;
}

void clear_char_list(CharList* list) {
    list->size = 0;
}

void destroy_char_list(CharList* list) {
    free(list->arr);
    list->arr = NULL;
    list->capacity = 0;
    list->size = 0;
}