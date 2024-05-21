#include "build_decision_tree.h"
#include "avl.h"
#include "data_format_conversion.h"
#include "data_query.h"
#include "heap_sort.h"
#include "text_tree.h"
#include "xalloc.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int data_queries_equal(DataQueryKey left, DataQueryKey right) {
  if (left.type != right.type) {
    return 0;
  }

  if (left.type == DQInt) {
    return left.key.data.integ == right.key.data.integ;
  }

  if (left.type == DQReal) {
    return left.key.data.real == right.key.data.real;
  }

  if (left.type == DQString) {

    char *l = left.key.data.str.ptr;
    char *r = right.key.data.str.ptr;
    for (; 1; ++l, ++r) {
      if (!*l && !*r) {
        return 1;
      }
      if (*l != *r) {
        return 0;
      }
    }
    return 1;
  }

  if (left.type == DQKeyword) {
    return left.key.word == right.key.word;
  }

  if (left.type == DQNone) {
    return 0;
  }

  if (left.type == DQList) {
    if (left.key.data.list.n != right.key.data.list.n) {
      return 0;
    }

    for (size_t i = 0; i < left.key.data.list.n; ++i) {
      if (!data_queries_equal(left.key.data.list.root[i],
                              right.key.data.list.root[i])) {
        return 0;
      }
    }
    return 1;
  }

  return 1;
}

int is_numerical(DataQueryKey key) {
  return key.type == DQInt || key.type == DQReal;
}

int data_queries_less(DataQueryKey left, DataQueryKey right) {
  if (left.type == DQInt) {
    return left.key.data.integ < right.key.data.integ;
  }

  if (left.type == DQReal) {
    return left.key.data.real < right.key.data.real;
  }

  return 0;
}

int compare_dqks_(DataQueryKey l, DataQueryKey r);

int data_queries_lesseq(DataQueryKey left, DataQueryKey right) {
  return data_queries_equal(left, right) || compare_dqks_(left, right) == -1;
}

DataQueryKey least_numerical_value(DataQueryType type) {
  DataQueryKey key;

  if (type == DQInt) {
    key.type = DQInt;
    key.key.data.integ = INT_FAST32_MIN;
  }

  else if (type == DQReal) {
    key.type = DQReal;
    key.key.data.real = -DBL_MAX;
  }

  return key;
}

DataQueryKey greatest_numerical_value(DataQueryType type) {
  DataQueryKey key;

  if (type == DQInt) {
    key.type = DQInt;
    key.key.data.integ = INT_FAST32_MAX;
  }

  else if (type == DQReal) {
    key.type = DQReal;
    key.key.data.real = DBL_MAX;
  }

  return key;
}

/* table 'view', no deep copying */
static DecisionTable get_filtered_table(DecisionTable table,
                                        int *numericAttributes, size_t attr,
                                        DataQueryKey smallestValue,
                                        DataQueryKey biggestValue) {
  assert(table.nr_rows >= 1);

  DecisionTable newTable;
  newTable.titles = table.titles;
  newTable.nr_columns = table.nr_columns;

  if (smallestValue.type == DQReal) {
    // printf("Smallest %lf\n", smallestValue.key.data.real);
    // printf("Biggest %lf\n", biggestValue.key.data.real);
  }

  size_t nrRows = 0;
  for (size_t i = 0; i < table.nr_rows; ++i) {
    if (data_queries_lesseq(smallestValue,
                            table.data[i * table.nr_columns + attr]) &&
        data_queries_lesseq(table.data[i * table.nr_columns + attr],
                            biggestValue)) {
      ++nrRows;
    }
  }

  if (nrRows == 0) {
    newTable.nr_rows = 0;
    newTable.data = NULL;
    if (table.data[0 * table.nr_columns + attr].type == DQReal) {
      fprintf(stderr, "Smallest %lf\n", smallestValue.key.data.real);
      fprintf(stderr, "Biggest %lf\n", biggestValue.key.data.real);
      for (size_t i = 0; i < table.nr_rows; ++i) {
        fprintf(stderr, "%lf\n",
                table.data[i * table.nr_columns + attr].key.data.real);
      }
    }

    fprintf(stderr, "Table size: %lu. Failed at line %u at file %s\n",
            table.nr_rows, __LINE__, __FILE__);
    return newTable;
  }

  newTable.data = xmalloc(sizeof(DataQueryKey) * nrRows * table.nr_columns,
                          __LINE__, __FILE__);
  if (!newTable.data) {
    memset(&newTable, 0, sizeof(newTable));
    return newTable;
  }

  newTable.nr_rows = nrRows;

  size_t c = 0;
  for (size_t i = 0; i < table.nr_rows; ++i) {
    if (data_queries_lesseq(smallestValue,
                            table.data[i * table.nr_columns + attr]) &&
        data_queries_lesseq(table.data[i * table.nr_columns + attr],
                            biggestValue)) {
      for (size_t j = 0; j < table.nr_columns; ++j) {
        newTable.data[c * table.nr_columns + j] =
            table.data[i * table.nr_columns + j];
      }
      ++c;
    }
  }

  return newTable;
}

static void free_filtered_table(DecisionTable table) { free(table.data); }

int compare_dqks_(DataQueryKey l, DataQueryKey r) {

  if (l.type == DQNone && r.type == DQNone) {
    return 0;
  }

  if (l.type == DQString && r.type == DQString) {
    unsigned long sl1 = strlen(l.key.data.str.ptr);
    unsigned long sl2 = strlen(r.key.data.str.ptr);
    if (sl1 != sl2) {
      return (sl1 < sl2) ? -1 : 1;
    }

    char *lc = l.key.data.str.ptr;
    char *rc = r.key.data.str.ptr;
    for (; 1; ++lc, ++rc) {
      if (!*lc && !*rc) {
        return 0;
      }
      if (*lc != *rc) {
        return *lc < *rc ? -1 : 1;
      }
    }

    return 0;
  }

  if (l.type == DQList && r.type == DQList) {
    if (l.key.data.list.n < r.key.data.list.n) {
      return -1;
    }
    if (l.key.data.list.n > r.key.data.list.n) {
      return 1;
    }

    for (size_t i = 0; i < l.key.data.list.n; ++i) {
      if (compare_dqks_(l.key.data.list.root[i], r.key.data.list.root[i]) !=
          0) {
        return compare_dqks_(l.key.data.list.root[i], r.key.data.list.root[i]);
      }
    }

    return 0;
  }

  if (l.type == DQInt && r.type == DQInt) {
    return (l.key.data.integ < r.key.data.integ)
               ? -1
               : (l.key.data.integ > r.key.data.integ ? 1 : 0);
  }

  if (l.type == DQReal && r.type == DQReal) {
    return (l.key.data.real < r.key.data.real)
               ? -1
               : (l.key.data.real > r.key.data.real ? 1 : 0);
  }

  if (l.type == DQKeyword && r.type == DQKeyword) {
    return (l.key.word < r.key.word) ? -1 : (l.key.word > r.key.word ? 1 : 0);
  }

  if (l.type < r.type)
    return -1;

  return 1;
}

int compare_dqks_ptr_(const void *l, const void *r) {
  assert(l && r);
  return compare_dqks_(*(DataQueryKey *)l, *(DataQueryKey *)r);
}

int compare_dqks(const void *left, const void *right) {
  const DataQueryKeyAvlNode *l = left;
  const DataQueryKeyAvlNode *r = right;
  assert(l && r);
  return compare_dqks_(l->value, r->value);
}

int compare_dqks2(const void *left, const void *right) {
  const DataQueryKeyAvlNode2 *l = left;
  const DataQueryKeyAvlNode2 *r = right;
  assert(l && r);
  return compare_dqks_(l->value, r->value);
}

static void calc_parents_1(DataQueryKeyAvlNode *root) {
  if (root->left) {
    root->left->parent_optional = root;
    calc_parents_1(root->left);
  }

  if (root->right) {
    root->right->parent_optional = root;
    calc_parents_1(root->right);
  }
}

static void calc_parents_2(DataQueryKeyAvlNode2 *root) {
  if (root->left) {
    root->left->parent_optional = root;
    calc_parents_2(root->left);
  }

  if (root->right) {
    root->right->parent_optional = root;
    calc_parents_2(root->right);
  }
}

static DataQueryKeyAvlNode *first_dfs(DataQueryKeyAvlNode *root) {
  return root;
}

static DataQueryKeyAvlNode *next_dfs(DataQueryKeyAvlNode *root) {
  if (root->left) {
    return root->left;
  }

  if (root->right) {
    return root->right;
  }

  DataQueryKeyAvlNode *parent = root->parent_optional;

  if (!parent) {
    return NULL;
  }

  while (!parent->right || !parent->left || parent->right == root) {
    root = parent;
    parent = root->parent_optional;

    if (!parent) {
      return NULL;
    }
  }

  return parent->right;
}

static DataQueryKeyAvlNode2 *first_dfs2(DataQueryKeyAvlNode2 *root) {
  return root;
}

static DataQueryKeyAvlNode2 *next_dfs2(DataQueryKeyAvlNode2 *root) {
  if (root->left) {
    return root->left;
  }

  if (root->right) {
    return root->right;
  }

  DataQueryKeyAvlNode2 *parent = root->parent_optional;

  if (!parent) {
    return NULL;
  }

  while (!parent->right || !parent->left || parent->right == root) {
    root = parent;
    parent = root->parent_optional;

    if (!parent) {
      return NULL;
    }
  }

  return parent->right;
}

static int calculate_entropy(double *result, DecisionTable table,
                             size_t target_attribute) {
  if (table.nr_rows == 0) {
    *result = 0;
    return 1;
  }

  DataQueryKeyAvlNode *avl_target_values_storage =
      xmalloc(table.nr_rows * sizeof(DataQueryKeyAvlNode), __LINE__, __FILE__);
  if (!avl_target_values_storage) {
    return 0;
  }

  memset(avl_target_values_storage, 0,
         table.nr_rows * sizeof(DataQueryKeyAvlNode));

  DataQueryKeyAvlNode *avl_tgt_val_storage_p = avl_target_values_storage;
  DataQueryKeyAvlNode *avl_tgt = NULL;

  /* "Calculate the entropy before the split" */

  double overall_entropy = 0;
  for (size_t i = 0; i < table.nr_rows; ++i) {
    DataQueryKey value_tgt =
        table.data[i * table.nr_columns + target_attribute];
    DataQueryKeyAvlNode *f2 = avl_find(avl_tgt, value_tgt, compare_dqks);
    if (!f2) {
      DataQueryKeyAvlNode *newf2 = avl_tgt_val_storage_p;
      avl_tgt =
          avl_insert(avl_tgt, value_tgt, avl_tgt_val_storage_p, compare_dqks);
      assert(newf2);
      ++avl_tgt_val_storage_p;
      f2 = newf2;
      f2->nr = 0;
      assert(avl_tgt);
    }
    ++f2->nr;
  }

  calc_parents_1(avl_tgt);

  for (DataQueryKeyAvlNode *n2 = first_dfs(avl_tgt); n2; n2 = next_dfs(n2)) {
    double prob = (double)n2->nr / table.nr_rows;
    overall_entropy -= prob * log2(prob);
  }

  free(avl_target_values_storage);
  *result = overall_entropy;
  return 1;
}

static int information_gain_for_split(double *result, DecisionTable data,
                                      DecisionTable left, DecisionTable right,
                                      size_t target_attribute) {

  size_t total_len = data.nr_rows;
  double left_prob = (double)left.nr_rows / total_len;
  double right_prob = (double)right.nr_rows / total_len;

  double calcentr1, calcentr2, calcentr3;
  if (!calculate_entropy(&calcentr1, data, target_attribute))
    return 0;
  if (!calculate_entropy(&calcentr2, left, target_attribute))
    return 0;
  if (!calculate_entropy(&calcentr3, right, target_attribute))
    return 0;
  *result = calcentr1 - (left_prob * calcentr2 + right_prob * calcentr3);
  return 1;
}

static size_t table_attr;
static int num_sort(const void *left, const void *right) {
  DataQueryKey *l = (DataQueryKey *)left;
  DataQueryKey *r = (DataQueryKey *)right;
  return compare_dqks_(l[table_attr], r[table_attr]);
}

/* assumes the table is sorted */
DataQueryKey *key_array_distinctize(DataQueryKey *table, size_t n,
                                    size_t *new_size) {
  assert(n >= 1);
  size_t nr_dstct = 1;
  DataQueryKey currKey = table[0];
  for (size_t i = 1; i < n; ++i) {
    if (!data_queries_equal(currKey, table[i])) {
      currKey = table[i];
      ++nr_dstct;
    }
  }

  DataQueryKey *new_arr =
      xcalloc(nr_dstct, sizeof(DataQueryKey), __LINE__, __FILE__);

  if (!new_arr) {
    fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
    return NULL;
  }

  size_t curr = 0;
  currKey = table[0];
  for (size_t i = 1; i < n; ++i) {
    if (!data_queries_equal(currKey, table[i])) {
      new_arr[curr] = currKey;
      currKey = table[i];
      ++curr;
    }
  }
  new_arr[curr] = currKey;

  *new_size = nr_dstct;
  return new_arr;
}

DataQueryKey get_key_avg(DataQueryKey a, DataQueryKey b) {
  if (a.type == DQInt) {
    if (a.key.data.integ > b.key.data.integ) {
      DataQueryKey temp = a;
      a = b;
      b = temp;
    }
    DataQueryKey avg;
    avg.key.data.integ =
        a.key.data.integ + (b.key.data.integ - a.key.data.integ) / 2;
    avg.type = DQInt;
    return avg;
  } else if (a.type == DQReal) {
    if (a.key.data.real > b.key.data.real) {
      DataQueryKey temp = a;
      a = b;
      b = temp;
    }
    DataQueryKey avg;
    avg.key.data.real =
        a.key.data.real + (b.key.data.real - a.key.data.real) / 2;
    avg.type = DQReal;
    return avg;
  }

  return a;
}

/* sorts table */
static int information_gain(double *result, DecisionTable table,
                            int *numericAttributes, size_t attr,
                            size_t target_attribute,
                            DataQueryKey *numericalSplitValue) {
  assert(table.nr_columns >= 1);
  if (numericAttributes[attr]) {
    /* numerical attribute measuring */

    table_attr = attr;
    heapsort(table.data, sizeof(DataQueryKey) * table.nr_columns, table.nr_rows,
             num_sort);

    DataQueryKey *src =
        xmalloc(table.nr_rows * sizeof(DataQueryKey), __LINE__, __FILE__);
    if (!src) {
      return 0;
    }

    for (size_t i = 0; i < table.nr_rows; ++i) {
      src[i] = table.data[i * table.nr_columns + attr];
    }

    assert(table.nr_rows > 1);

    size_t dstsz;
    DataQueryKey *distinct = key_array_distinctize(src, table.nr_rows, &dstsz);
    if (!distinct) {
      free(src);
      return 0;
    }

    free(src);

    if (dstsz == 1) {
      *numericalSplitValue = distinct[0];
      free(distinct);
      *result = -DBL_MAX;
      return 1;
    }

    size_t split_index = dstsz;
    double max_gain;
    DataQueryKey avg_max;

    // printf("--- (num) TABLE SIZE = %ld -----\n", table.nr_rows);
    /* for each adjacent avg */
    for (size_t i = 0; i < dstsz - 1; ++i) {
      DataQueryKey avg = get_key_avg(distinct[i], distinct[i + 1]);
      assert(avg.type == DQReal);
      DecisionTable left = get_filtered_table(
          table, numericAttributes, attr, least_numerical_value(avg.type), avg);
      if (!left.nr_rows) {
        free(distinct);
        fprintf(stderr, "Distinct size: %lu Failed at line %u at file %s\n",
                dstsz, __LINE__, __FILE__);
        return 0;
      }
      DecisionTable right =
          get_filtered_table(table, numericAttributes, attr, avg,
                             greatest_numerical_value(avg.type));
      if (!right.nr_rows) {
        free_filtered_table(left);
        free(distinct);
        fprintf(stderr, "Distinct size: %lu Failed at line %u at file %s\n",
                dstsz, __LINE__, __FILE__);
        return 0;
      }
      double gain;
      int scss = information_gain_for_split(&gain, table, left, right,
                                            target_attribute);
      if (!scss) {
        free(distinct);
        free_filtered_table(left);
        free_filtered_table(right);
        return 0;
      }

      if (split_index == dstsz || gain > max_gain) {
        max_gain = gain;
        split_index = i;
        avg_max = avg;
      }
      free_filtered_table(left);
      free_filtered_table(right);
    }

    *result = max_gain;
    assert(split_index < dstsz);
    assert(avg_max.type == DQReal);
    *numericalSplitValue = avg_max;
    free(distinct);
    return 1;

  } else {
    /* non-numerical attribute measuring */

    /* copied from numerical */
    DataQueryKey *src =
        xmalloc(table.nr_rows * sizeof(DataQueryKey), __LINE__, __FILE__);
    if (!src) {
      return 0;
    }

    for (size_t i = 0; i < table.nr_rows; ++i) {
      src[i] = table.data[i * table.nr_columns + attr];
    }

    assert(table.nr_rows > 1);

    size_t dstsz;
    DataQueryKey *distinct = key_array_distinctize(src, table.nr_rows, &dstsz);
    if (!distinct) {
      free(src);
      return 0;
    }

    free(src);

    size_t split_index = dstsz;
    double max_gain;
    DataQueryKey avg_max;
    /* ^ copied from numerical */

    for (size_t i = 0; i < dstsz; ++i) {
      DecisionTable subdata = get_filtered_table(table, numericAttributes, attr,
                                                 distinct[i], distinct[i]);
      if (!subdata.nr_rows) {
        assert(!subdata.nr_columns);
        free(distinct);
        fprintf(stderr, "Distinct size: %lu Failed at line %u at file %s\n",
                dstsz, __LINE__, __FILE__);
        return 0;
      }
      double gain1;
      int scss = calculate_entropy(&gain1, table, target_attribute);
      if (!scss) {
        free_filtered_table(subdata);
        free(distinct);
        return 0;
      }
      double gain2;
      scss = calculate_entropy(&gain2, subdata, target_attribute);
      if (!scss) {
        free_filtered_table(subdata);
        free(distinct);
        return 0;
      }

      free_filtered_table(subdata);
      double gain = gain1 - gain2;
      if (gain > max_gain || split_index == dstsz) {
        max_gain = gain;
        split_index = i;
      }
    }

    *result = max_gain;
    *numericalSplitValue = distinct[split_index];
    assert(split_index < dstsz);
    free(distinct);
    return 1;
  }
  return 0;
}

static int get_best_attribute(size_t *result, DecisionTable table,
                              int *numericAttributes, int *attributes,
                              size_t nr_attr, size_t tgt_attr,
                              DataQueryKey *numericalSplitValue) {

  double max_gain = 0;
  size_t best_attr_now = table.nr_columns;
  DataQueryKey currNumericalSplitValue;

  for (size_t i = 0; i < table.nr_columns; ++i) {
    if (!attributes[i]) {
      double gain;
      int ss = information_gain(&gain, table, numericAttributes, i, tgt_attr,
                                &currNumericalSplitValue);

      if (!ss) {
        return 0;
      }
      if (numericAttributes[i]) {
        assert(currNumericalSplitValue.type == DQReal);
      }
      if (gain > max_gain || best_attr_now == table.nr_columns) {
        max_gain = gain;
        best_attr_now = i;
        *numericalSplitValue = currNumericalSplitValue;
      }
    }
  }

  if (best_attr_now == table.nr_columns) {
    return 0;
  }

  *result = best_attr_now;
  return 1;
}

TextTreeNode build_decision_tree(DecisionTable table, int *attributes,
                                 int *numericAttributes, size_t nr_attr,
                                 size_t depth, size_t target_attribute,
                                 char *parent_text, TextTreeNode *parent) {
  TextTreeNode none;
  memset(&none, 0, sizeof(none));
  /* no data */

  if (table.nr_rows == 0) {
    fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
    return none;
  }

  /* all are the same */
  int same = 1;
  DataQueryKey t = table.data[0 * table.nr_columns + target_attribute];
  for (size_t i = 1; i < table.nr_rows; ++i) {
    if (!data_queries_equal(table.data[i * table.nr_columns + target_attribute],
                            t)) {
      same = 0;
      break;
    }
  }

  if (same) {
    TextTreeNode node;
    DataQueryKey dominantClass =
        table.data[0 * table.nr_columns + target_attribute];
    DataQueryKey strCvt;
    int scss = convertToStr(&strCvt, dominantClass, 0, 0);
    if (!scss) {
      fprintf(stderr, "STRING Failed at line %u at file %s\n", __LINE__,
              __FILE__);
      return none;
    }

    node.node_text = xmalloc(strCvt.key.data.str.n + 1, __LINE__, __FILE__);
    if (!node.node_text) {
      freeKey(&strCvt);
      fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
      return none;
    }

    strncpy(node.node_text, strCvt.key.data.str.ptr, strCvt.key.data.str.n);
    node.node_text[strCvt.key.data.str.n] = 0;

    node.parent_text = parent_text;
    node.parent = parent;
    node.children = NULL;
    node.nr_children = 0;
    freeKey(&strCvt);
    return node;
  }

  /* no attributes or zero depth */
  if (nr_attr == table.nr_columns || depth == 0) {
    TextTreeNode node;

    /* the simplest way to calculate mode element */
    int *modeArr = xmalloc(table.nr_rows * sizeof(int), __LINE__, __FILE__);
    if (!modeArr) {
      fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
      return none;
    }
    memset(modeArr, 0, sizeof(int) * table.nr_rows);

    DataQueryKey maxNowKey;
    size_t max_count = 0;
    size_t all_count = 0;
    while (all_count != table.nr_rows) {
      size_t count = 0;
      size_t idx = 0;

      assert(all_count != table.nr_rows);
      while (modeArr[idx]) {
        ++idx;
        assert(idx < table.nr_rows);
      }
      assert(idx * table.nr_columns + target_attribute <
             table.nr_columns * table.nr_rows);
      DataQueryKey nowKey =
          table.data[idx * table.nr_columns + target_attribute];
      ++count;

      modeArr[idx] = 1;
      ++all_count;

      while (idx != table.nr_rows) {
        if (!modeArr[idx] &&
            data_queries_equal(
                nowKey,
                table.data[idx * table.nr_columns + target_attribute])) {
          modeArr[idx] = 1;
          ++count;
          ++all_count;
        }
        ++idx;
      }
      if (max_count < count) {
        max_count = count;
        maxNowKey = nowKey;
      }
    }

    free(modeArr);

    DataQueryKey dominantClass = maxNowKey;
    DataQueryKey strCvt;
    strCvt.type = DQNone;
    int scss = convertToStr(&strCvt, dominantClass, 0, 0);
    if (!scss) {
      fprintf(stderr, "STRING Failed at line %u at file %s\n", __LINE__,
              __FILE__);
      return none;
    }

    node.node_text = xmalloc(strCvt.key.data.str.n + 1, __LINE__, __FILE__);
    if (!node.node_text) {
      freeKey(&strCvt);
      fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
      return none;
    }

    strncpy(node.node_text, strCvt.key.data.str.ptr, strCvt.key.data.str.n);
    node.node_text[strCvt.key.data.str.n] = 0;

    node.parent_text = parent_text;
    node.parent = parent;
    node.children = NULL;
    node.nr_children = 0;
    freeKey(&strCvt);
    return node;
  }

  size_t best_attr;
  DataQueryKey numericalSplitValue;

  {
    int tmp = 0;
    for (int i = 0; i < table.nr_columns; ++i) {
      if (attributes[i]) {
        ++tmp;
      }
    }
    assert(tmp == nr_attr);
  }

  int ss = get_best_attribute(&best_attr, table, numericAttributes, attributes,
                              nr_attr, target_attribute, &numericalSplitValue);

  if (!ss) {
    return none;
  }

  if (numericAttributes[best_attr]) {
    assert(numericalSplitValue.type == DQReal);
  }

  TextTreeNode newNode;

  newNode.node_text =
      xmalloc(strlen(table.titles[best_attr]) + 1, __LINE__, __FILE__);
  if (!newNode.node_text) {
    fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
    return none;
  }

  newNode.node_text[strlen(table.titles[best_attr])] = 0;
  strncpy(newNode.node_text, table.titles[best_attr],
          strlen(table.titles[best_attr]));

  if (!numericAttributes[best_attr]) {
    DataQueryKey *bestAttrValues = NULL;

    bestAttrValues =
        xmalloc(table.nr_rows * sizeof(DataQueryKey), __LINE__, __FILE__);
    if (!bestAttrValues) {
      free(newNode.node_text);
      fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
      return none;
    }

    for (size_t i = 0; i < table.nr_rows; ++i) {
      bestAttrValues[i] = table.data[i * table.nr_columns + best_attr];
    }

    heapsort(bestAttrValues, sizeof(DataQueryKey), table.nr_rows,
             compare_dqks_ptr_);

    DataQueryKey currentValue = bestAttrValues[0];

    TextTreeNode *newNodeChildren =
        xmalloc(sizeof(TextTreeNode) * table.nr_rows, __LINE__, __FILE__);
    if (!newNodeChildren) {

      free(bestAttrValues);
      free(newNode.node_text);
      fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
      return none;
    }
    memset(newNodeChildren, 0, sizeof(TextTreeNode) * table.nr_rows);
    size_t nrNewChildren = 0;

    for (size_t i = 0; i < table.nr_rows; ++i) {

      if (i == 0 || !data_queries_equal(currentValue, bestAttrValues[i])) {
        DecisionTable subdata =
            get_filtered_table(table, numericAttributes, best_attr,
                               bestAttrValues[i], bestAttrValues[i]);

        if (subdata.titles == NULL || subdata.data == NULL) {
          for (size_t j = 0; j < table.nr_rows; ++j) {
            free_text_tree(newNodeChildren[j]);
          }
          free(newNodeChildren);
          free(bestAttrValues);
          free(newNode.node_text);
          fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
          return none;
        }

        assert(subdata.nr_rows != 0);
        if (1) {

          DataQueryKey newStrPtr;
          int scss = convertToStr(&newStrPtr, bestAttrValues[i], 0, 0);

          if (!scss) {
            for (size_t j = 0; j < table.nr_rows; ++j) {
              free_text_tree(newNodeChildren[j]);
            }
            free(newNodeChildren);
            free_filtered_table(subdata);
            free(bestAttrValues);
            free(newNode.node_text);
            fprintf(stderr, "STRING Failed at line %u at file %s\n", __LINE__,
                    __FILE__);
            return none;
          }

          TextTreeNode *node = &newNodeChildren[nrNewChildren];
          if (!attributes[best_attr])
            ++nr_attr;
          attributes[best_attr] = 1;

          *node = build_decision_tree(subdata, attributes, numericAttributes,
                                      nr_attr, depth - 1, target_attribute,
                                      newStrPtr.key.data.str.ptr, &newNode);

          --nr_attr;
          attributes[best_attr] = 0;

          if (!node->nr_children && !node->node_text && !node->children) {
            freeKey(&newStrPtr);
            for (size_t j = 0; j < table.nr_rows; ++j) {
              free_text_tree(newNodeChildren[j]);
            }
            free(newNodeChildren);
            free_filtered_table(subdata);
            free(bestAttrValues);
            free(newNode.node_text);
            fprintf(stderr, "Failed at line %u at file %s\n", __LINE__,
                    __FILE__);
            return none;
          }

          /*--nr_attr;
          attributes[best_attr] = 0;*/
          ++nrNewChildren;
        }
        currentValue = bestAttrValues[i];
        free_filtered_table(subdata);
      }
    }

    if (nrNewChildren > 1) {
      newNode.children = newNodeChildren;
      newNode.nr_children = nrNewChildren;
      newNode.parent_text = parent_text;
      newNode.parent = parent;
      free(bestAttrValues);
    }

    else if (nrNewChildren == 0) {
      for (size_t j = 0; j < table.nr_rows; ++j) {
        free_text_tree(newNodeChildren[j]);
      }
      free(newNodeChildren);
      free(bestAttrValues);
      newNode.parent = parent;
      newNode.parent_text = parent_text;
    }

    else if (nrNewChildren == 1) {
      newNode.children = newNodeChildren;
      newNode.nr_children = nrNewChildren;
      free(bestAttrValues);
      free(newNode.node_text);
      newNode = newNode.children[0];
      free(newNode.parent_text);
      newNode.parent_text = parent_text;
      newNode.parent = parent;
      free(newNodeChildren);
    }
    return newNode;

  } else {
    /* numerical */

    TextTreeNode *newNodeChildren =
        xmalloc(sizeof(TextTreeNode) * 2, __LINE__, __FILE__);
    if (!newNodeChildren) {

      free(newNode.node_text);
      return none;
    }
    memset(newNodeChildren, 0, sizeof(TextTreeNode) * 2);
    size_t nrNewChildren = 0;

    for (size_t i = 0; i < 2; ++i) {

      if (1) {
        assert(numericalSplitValue.type == DQReal);
        DecisionTable subdata = get_filtered_table(
            table, numericAttributes, best_attr,
            i == 0 ? least_numerical_value(numericalSplitValue.type)
                   : numericalSplitValue,
            i == 0 ? numericalSplitValue
                   : greatest_numerical_value(numericalSplitValue.type));

        if (subdata.titles == NULL || subdata.data == NULL) {
          for (size_t j = 0; j < 2; ++j) {
            free_text_tree(newNodeChildren[j]);
          }
          free(newNodeChildren);
          free(newNode.node_text);
          fprintf(stderr, "Failed at line %u at file %s\n", __LINE__, __FILE__);
          return none;
        }

        assert(subdata.nr_rows != 0);
        if (1) {

          int best_attr_same = 1;
          for (size_t i = 0; i < subdata.nr_rows - 1; ++i) {
            if (subdata.data[best_attr + i * subdata.nr_columns]
                    .key.data.real !=
                subdata.data[best_attr + (i + 1) * subdata.nr_columns]
                    .key.data.real) {
              best_attr_same = 0;
              break;
            }
          }

          DataQueryKey newStrPtr;
          int scss = convertToStr(&newStrPtr, numericalSplitValue, 0, 3);

          if (!scss) {
            for (size_t j = 0; j < 2; ++j) {
              free_text_tree(newNodeChildren[j]);
            }
            free(newNodeChildren);
            free_filtered_table(subdata);
            free(newNode.node_text);
            fprintf(stderr, "STRING Failed at line %u at file %s\n", __LINE__,
                    __FILE__);
            return none;
          }

          newStrPtr.key.data.str.ptr[2] = ' ';
          if (i == 0) {
            newStrPtr.key.data.str.ptr[0] = '<';
            newStrPtr.key.data.str.ptr[1] = ' ';
          } else {
            newStrPtr.key.data.str.ptr[0] = '>';
            newStrPtr.key.data.str.ptr[1] = ' ';
          }

          TextTreeNode *node = &newNodeChildren[nrNewChildren];

          if (best_attr_same) {
            if (!attributes[best_attr])
              ++nr_attr;
            attributes[best_attr] = 1;
          }

          *node = build_decision_tree(subdata, attributes, numericAttributes,
                                      nr_attr, depth - 1, target_attribute,
                                      newStrPtr.key.data.str.ptr, &newNode);

          if (best_attr_same) {
            --nr_attr;
            attributes[best_attr] = 0;
          }

          if (!node->nr_children && !node->node_text && !node->children) {
            freeKey(&newStrPtr);
            for (size_t j = 0; j < 2; ++j) {
              free_text_tree(newNodeChildren[j]);
            }
            free(newNodeChildren);
            free_filtered_table(subdata);
            free(newNode.node_text);
            return none;
          }

          /*--nr_attr;
          attributes[best_attr] = 0;*/
          ++nrNewChildren;
        }
        free_filtered_table(subdata);
      }
    }

    if (nrNewChildren > 1) {
      newNode.children = newNodeChildren;
      newNode.nr_children = nrNewChildren;
      newNode.parent_text = parent_text;
      newNode.parent = parent;
    }

    else if (nrNewChildren == 0) {
      for (size_t j = 0; j < 2; ++j) {
        free_text_tree(newNodeChildren[j]);
      }
      free(newNodeChildren);
      newNode.parent = parent;
      newNode.parent_text = parent_text;
    }

    else if (nrNewChildren == 1) {
      newNode.children = newNodeChildren;
      newNode.nr_children = nrNewChildren;
      free(newNode.node_text);
      newNode = newNode.children[0];
      free(newNode.parent_text);
      newNode.parent_text = parent_text;
      newNode.parent = parent;
      free(newNodeChildren);
    }
    return newNode;
  }

  return newNode;
}