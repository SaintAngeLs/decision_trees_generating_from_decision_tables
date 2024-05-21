#include "data_format_conversion.h"
#include "data_query.h"
#include "xalloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_decision_table(DecisionTable table) {
  for (size_t j = 0; j < table.nr_columns; ++j) {
    printf("[%s] ", table.titles[j]);
  }
  printf("\n");

  for (size_t i = 0; i < table.nr_rows; ++i) {
    for (size_t j = 0; j < table.nr_columns; ++j) {

      printf("[%s] ", table.data[i * table.nr_columns + j].key.data.str.ptr);
    }
    printf("\n");
  }
}

void free_data_table(DataQueryKey *data, size_t width, size_t height) {
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      freeKey(data + (i * width + j));
    }
  }
  free(data);
}

void free_decision_table(DecisionTable table, int with_titles) {
  free_data_table(table.data, table.nr_columns, table.nr_rows);
  if (with_titles) {
    for (size_t i = 0; i < table.nr_columns; ++i) {
      free(table.titles[i]);
    }
    free(table.titles);
  }
}

/*TODO check*/
DecisionTable create_decision_table_from_csv(CsvData *csv,
                                             DataQueryType *types) {
  DecisionTable dectab;
  memset(&dectab, 0, sizeof(dectab));

  char **titles = xmalloc(sizeof(char *) * csv->nr_columns, __LINE__, __FILE__);
  if (!titles) {
    return dectab;
  }

  DataQueryKey *newData =
      xmalloc((csv->nr_rows - 1) * csv->nr_columns * sizeof(DataQueryKey),
              __LINE__, __FILE__);
  if (!newData) {
    free(titles);
    return dectab;
  }

  memset(titles, 0, sizeof(char *) * csv->nr_columns);

  for (size_t i = 0; i < csv->nr_columns; ++i) {
    titles[i] = xmalloc(strlen(csv->data[i]) + 1, __LINE__, __FILE__);

    if (!titles[i]) {
      for (size_t ii = 0; ii < csv->nr_columns; ++ii) {
        free(titles[ii]);
      }
      free(titles);
      free(newData);
      return dectab;
    }

    strncpy(titles[i], csv->data[i], strlen(csv->data[i]));
    titles[i][strlen(csv->data[i])] = 0;
  }

  memset(newData, 0, sizeof(DataQueryKey));

  for (size_t i = 1; i < csv->nr_rows; ++i) {
    for (size_t j = 0; j < csv->nr_columns; ++j) {

      /* TODO: check csv->data non null */

      if (types[j] == DQInt) {
        newData[(i - 1) * csv->nr_columns + j].key.data.integ =
            strtol(csv->data[i * csv->nr_columns + j], NULL, 10);
        newData[(i - 1) * csv->nr_columns + j].type = DQInt;
      } else if (types[j] == DQReal) {
        newData[(i - 1) * csv->nr_columns + j].key.data.real =
            strtod(csv->data[i * csv->nr_columns + j], NULL);
        newData[(i - 1) * csv->nr_columns + j].type = DQReal;
      } else { /* str */

        newData[(i - 1) * csv->nr_columns + j].key.data.str.ptr = xmalloc(
            strlen(csv->data[i * csv->nr_columns + j]) + 1, __LINE__, __FILE__);

        if (!newData[(i - 1) * csv->nr_columns + j].key.data.str.ptr) {
          for (size_t ii = 0; ii < csv->nr_columns; ++ii) {
            free(titles[ii]);
          }
          free(titles);
          free_data_table(newData, csv->nr_columns, csv->nr_rows - 1);
          free(newData);
          return dectab;
        }

        newData[(i - 1) * csv->nr_columns + j]
            .key.data.str.ptr[strlen(csv->data[i * csv->nr_columns + j])] = 0;
        strncpy(newData[(i - 1) * csv->nr_columns + j].key.data.str.ptr,
                csv->data[i * csv->nr_columns + j],
                strlen(csv->data[i * csv->nr_columns + j]));
        newData[(i - 1) * csv->nr_columns + j].type = DQString;
      }
    }
  }

  dectab.data = newData;
  dectab.titles = titles;
  dectab.nr_columns = csv->nr_columns;
  dectab.nr_rows = csv->nr_rows - 1;
  return dectab;
}

DecisionTable create_decision_table_from_parsed_tree(StringTreeNode tree,
                                                     char **titles,
                                                     DataQueryKey **instrPtr,
                                                     size_t *instr_size_ptr,
                                                     size_t nr_columns) {

  DecisionTable dectab;
  memset(&dectab, 0, sizeof(dectab));

  /* first */
  size_t n;
  DataQueryKey firstKey =
      miniml_data_query(instrPtr[0], instr_size_ptr[0], tree);
  if (firstKey.type != DQList) {
    freeKey(&firstKey);
    return dectab;
  }

  n = firstKey.key.data.list.n;

  DataQueryKey *newData =
      xmalloc(n * nr_columns * sizeof(DataQueryKey), __LINE__, __FILE__);
  if (!newData) {
    freeKey(&firstKey);
    return dectab;
  }

  memset(newData, 0, sizeof(DataQueryKey));

  /*first*/
  for (size_t i = 0; i < n; ++i) {
    /* move */
    newData[i * nr_columns + 0] = firstKey.key.data.list.root[i];
  }
  free(firstKey.key.data.list.root);

  for (size_t j = 1; j < nr_columns; ++j) {
    firstKey = miniml_data_query(instrPtr[j], instr_size_ptr[j], tree);
    if (firstKey.type != DQList || firstKey.key.data.list.n != n) {
      freeKey(&firstKey);
      free_data_table(newData, nr_columns, n);
      return dectab;
    }
    for (size_t i = 0; i < n; ++i) {
      /* move */
      newData[i * nr_columns + j] = firstKey.key.data.list.root[i];
    }
    free(firstKey.key.data.list.root);
  }

  dectab.titles = titles;
  dectab.data = newData;
  dectab.nr_columns = nr_columns;
  dectab.nr_rows = n;
  return dectab;
}

void convert_column_to_int(DecisionTable table, size_t column) {
  for (size_t i = 0; i < table.nr_rows; ++i) {
    DataQueryKey newKey;
    int scss =
        convertToInt(&newKey, table.data[i * table.nr_columns + column], 0);
    assert(scss);
    freeKey(&table.data[i * table.nr_columns + column]);
    table.data[i * table.nr_columns + column] = newKey;
  }
}

void convert_column_to_real(DecisionTable table, size_t column) {
  for (size_t i = 0; i < table.nr_rows; ++i) {
    DataQueryKey newKey;
    int scss =
        convertToReal(&newKey, table.data[i * table.nr_columns + column], 0);
    assert(scss);
    freeKey(&table.data[i * table.nr_columns + column]);
    table.data[i * table.nr_columns + column] = newKey;
  }
}