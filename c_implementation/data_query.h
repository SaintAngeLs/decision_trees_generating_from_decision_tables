#ifndef MINIML_DATA_QUERY_H
#define MINIML_DATA_QUERY_H
#include "string_tree.h"
#include <stddef.h>
#include <stdint.h>

typedef enum DataQueryKeyType_ {
  DQNone, /* intended to be first */
  DQKeyword,
  DQInt,
  DQString,
  DQReal,
  DQList,
} DataQueryType;

typedef enum DataQueryKeyword_ {
  DQK_toInt,
  DQK_toFloat,
  DQK_toStr,

  DQK_propId,  /* refer to property */
  DQK_foreach, /* refer to all children */
  DQK_up,      /* go back to root */
  DQK_changeLvl,

  DQK_fetch,

  /* List -> Value */
  DQK_sum,
  DQK_multiplication,
  DQK_min,
  DQK_max,
  DQK_count,
  DQK_avg,
  DQK_union,
  DQK_distinct
} DataQueryKeyword;

typedef union DataImpl_ {
  int_fast32_t integ;
  double real;
  struct {
    char *ptr; /* with null terminator */
    size_t n;  /* without null terminator */
  } str;
  struct {
    struct DataQueryKey_ *root;
    size_t n;
  } list;
} DataImpl;

typedef struct DataQueryKey_ {

  union {
    DataQueryKeyword word;
    DataImpl data;
  } key;

  DataQueryType type;
} DataQueryKey;

void freeKey(DataQueryKey *data);

DataQueryKey copyKeyStructure(DataQueryKey data);

int convertToStr(DataQueryKey *dst, DataQueryKey data, size_t lvl,
                 size_t offset);

/* inverted Polish notation */
/* arr: integers or words only */
/* assumed list returned */
DataQueryKey miniml_data_query(DataQueryKey *arr, size_t size,
                               StringTreeNode tree);

int convertToInt(DataQueryKey *dst, DataQueryKey data, size_t lvl);
int convertToReal(DataQueryKey *dst, DataQueryKey data, size_t lvl);

#endif