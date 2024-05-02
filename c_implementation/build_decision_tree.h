#ifndef MINIML_BUILD_DECISION_TREE_H
#define MINIML_BUILD_DECISION_TREE_H
#include "decision_table.h"
#include "text_tree.h"
#include <stdint.h>

/* non_comparible: Don't care if string or ignored (NULL for default) */
/* select columns to ignore (NULL for default) */
TextTreeNode* build_decision_tree(DecisionTable* table, int* non_comparable, int* ignore, size_t target);

#endif /* MINIML_BUILD_DECISION_TREE_H */ 