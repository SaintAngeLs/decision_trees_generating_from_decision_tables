#ifndef MINIML_BUILD_DECISION_TREE_H
#define MINIML_BUILD_DECISION_TREE_H
#include "data_format_conversion.h"
#include "text_tree.h"
#include <stdint.h>

/** \brief Build a decision tree from a decision table.
 *
 * @param table A decision table
 * @returns A decision tree created from the decision table
 */
TextTreeNode build_decision_tree(DecisionTable table,
                                 int *attributes, /* masked attributes */
                                 int *numericAttributes, size_t nr_attr,
                                 size_t depth, size_t target_attribute,
                                 char *parent_text, TextTreeNode *parent);

int compare_dqks_ptr_(const void *l, const void *r);

int data_queries_equal(DataQueryKey left, DataQueryKey right);

#endif /* MINIML_BUILD_DECISION_TREE_H */