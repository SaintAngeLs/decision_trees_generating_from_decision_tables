#include "build_decision_tree.h"
#include "avl.h"
#include "data_format_conversion.h"
#include "data_query.h"
#include "heap_sort.h"
#include "text_tree.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int data_queries_equal(DataQueryKey left, DataQueryKey right);

/* table 'view', no deep copying */
static DecisionTable get_filtered_table(DecisionTable table, size_t best_attr, DataQueryKey value) {
    DecisionTable newTable;
    newTable.titles = table.titles;
    newTable.nr_columns = table.nr_columns;

    size_t nrRows = 0;
    for (size_t i = 0; i < table.nr_rows; ++i) {
        if (data_queries_equal(value, table.data[i*table.nr_columns+best_attr])) {
            ++nrRows;
        }
    }

    if (nrRows == 0) {
        newTable.nr_rows = 0;
        newTable.data = NULL;
        return newTable;
    }

    newTable.data = malloc(sizeof(DataQueryKey) * nrRows * table.nr_columns);
    if (!newTable.data) {
        newTable.titles = NULL;
        return newTable;
    }

    newTable.nr_rows = nrRows;

    size_t c = 0;
    for (size_t i = 0; i < table.nr_rows; ++i) {
        if (data_queries_equal(value, table.data[i*table.nr_columns+best_attr])) {
            for (size_t j = 0; j < table.nr_columns; ++j) {
                newTable.data[c*table.nr_columns+j] = table.data[i*table.nr_columns+j];
            }
            ++c;
        }
    }

    return newTable;
}

static void free_filtered_table(DecisionTable table) {
    free(table.data);
}

int compare_dqks_(DataQueryKey l, DataQueryKey r) {
    union {
        DataQueryKey in;
        intmax_t out;
    } conv;

    intmax_t lc, rc;
    conv.in = l;
    lc = conv.out;

    conv.in = r;
    rc = conv.out;

    return (lc < rc) ? -1 : (lc > rc ? 1 : 0);

    if (l.type == DQNone && r.type == DQNone) {
        return 0;
    }

    if (l.type == DQString && r.type ==DQString) {
        return strcmp(l.key.data.str.ptr, r.key.data.str.ptr);
    }

    if (l.type == DQList && r.type == DQList) {
        if (l.key.data.list.n < r.key.data.list.n) {
            return -1;
        }
        if (l.key.data.list.n > r.key.data.list.n) {
            return 1;
        }

        for (size_t i = 0; i < l.key.data.list.n; ++i) {
            if (compare_dqks_(l.key.data.list.root[i], r.key.data.list.root[i]) != 0) {
                return compare_dqks_(l.key.data.list.root[i], r.key.data.list.root[i]);
            }
        }

        return 0;
    }

    if (l.type == DQInt && r.type == DQInt) {
        return (l.key.data.integ < r.key.data.integ) ? -1 : (l.key.data.integ > r.key.data.integ ? 1 : 0);
    }

    if (l.type == DQReal && r.type == DQReal) {
        return (l.key.data.real < r.key.data.real) ? -1 : (l.key.data.real > r.key.data.real ? 1 : 0);
    }

    if (l.type == DQKeyword && r.type == DQKeyword) {
        return (l.key.word < r.key.word) ? -1 : (l.key.word > r.key.word ? 1 : 0);
    }

    if (l.type < r.type)
        return -1;

    return 1;
}

int compare_dqks_ptr_(const void* l, const void* r) {
   assert(l&&r);
    return compare_dqks_(*(DataQueryKey*)l, *(DataQueryKey*)r);
}

int compare_dqks(const void* left, const void* right) {
    const DataQueryKeyAvlNode* l = left;
    const DataQueryKeyAvlNode* r = right;
assert(l&&r);
    return compare_dqks_(l->value, r->value);
}

int compare_dqks2(const void* left, const void* right) {
    const DataQueryKeyAvlNode2* l = left;
    const DataQueryKeyAvlNode2* r = right;
    assert(l&&r);
    return compare_dqks_(l->value, r->value);
}
















static void calc_parents_1(DataQueryKeyAvlNode* root) {
    if (root->left) {
        root->left->parent_optional = root;
        calc_parents_1(root->left);
    }

    if (root->right) {
        root->right->parent_optional = root;
        calc_parents_1(root->right);
    }
}

static void calc_parents_2(DataQueryKeyAvlNode2* root) {
    if (root->left) {
        root->left->parent_optional = root;
        calc_parents_2(root->left);
    }

    if (root->right) {
        root->right->parent_optional = root;
        calc_parents_2(root->right);
    }
}

static DataQueryKeyAvlNode* first_dfs(DataQueryKeyAvlNode* root) {
    return root;
} 

static DataQueryKeyAvlNode* next_dfs(DataQueryKeyAvlNode* root) {
    if (root->left) {
        return root->left;
    }

    if (root->right) {
        return root->right;
    }

    DataQueryKeyAvlNode* parent = root->parent_optional;

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

static DataQueryKeyAvlNode2* first_dfs2(DataQueryKeyAvlNode2* root) {
    return root;
} 

static DataQueryKeyAvlNode2* next_dfs2(DataQueryKeyAvlNode2* root) {
    if (root->left) {
        return root->left;
    }

    if (root->right) {
        return root->right;
    }

    DataQueryKeyAvlNode2* parent = root->parent_optional;

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















static int information_gain(double* result, DecisionTable table, size_t attr, size_t target_attribute) {
    double attribute_entropy = 0;

    DataQueryKeyAvlNode* avl_values_storage = malloc(table.nr_rows * sizeof(DataQueryKeyAvlNode));
    if (!avl_values_storage) {
        return 0;
    }

    DataQueryKeyAvlNode* avl_target_values_storage = malloc(table.nr_rows * sizeof(DataQueryKeyAvlNode));
    if (!avl_target_values_storage) {
        free(avl_values_storage);
        return 0;
    }

    DataQueryKeyAvlNode2* avl_values_storage2 = malloc(table.nr_rows * sizeof(DataQueryKeyAvlNode2));
    if (!avl_values_storage2) {
        free(avl_target_values_storage);
        free(avl_values_storage);
        return 0;
    }

    memset(avl_values_storage, 0, table.nr_rows * sizeof(DataQueryKeyAvlNode));
    memset(avl_target_values_storage, 0, table.nr_rows * sizeof(DataQueryKeyAvlNode));
    memset(avl_values_storage2, 0, table.nr_rows * sizeof(DataQueryKeyAvlNode2));

    DataQueryKeyAvlNode* avl_val_storage_p = avl_values_storage;
    DataQueryKeyAvlNode* avl_tgt_val_storage_p = avl_target_values_storage;
    DataQueryKeyAvlNode2* avl_val_storage_p2 = avl_values_storage2;

    DataQueryKeyAvlNode2* avl = NULL;
    DataQueryKeyAvlNode* avl_tgt = NULL;

    /* "Calculate the frequency of each value in the target attribute" */

    for (size_t i = 0; i < table.nr_rows; ++i) {
        DataQueryKey value = table.data[i*table.nr_columns + attr];
        DataQueryKey value_tgt = table.data[i*table.nr_columns + target_attribute];

        DataQueryKeyAvlNode2* f = avl_find2(avl, value, compare_dqks2);
        if (!f) {
            avl = avl_insert2(avl, value, avl_val_storage_p2, 
            compare_dqks2);
            DataQueryKeyAvlNode2* newf = avl_val_storage_p2;
            assert(newf);
            ++avl_val_storage_p2;
            f = newf;
            f->nr = 0;
            assert(avl);
        }
        
        DataQueryKeyAvlNode* f2 = avl_find(f->subavl, value_tgt, compare_dqks);
        if (!f2) {
            DataQueryKeyAvlNode* newf2 = avl_val_storage_p;
            f->subavl = avl_insert(f->subavl, value_tgt,
             avl_val_storage_p, compare_dqks);
             assert(newf2);
            ++avl_val_storage_p;
            f2 = newf2;
            f2->nr = 0;
            assert(f->subavl);
        }
        
        ++f->nr;
        ++f2->nr;
    }

    /* "Calculate the entropy of each subset of records weighted by their probability" */

    calc_parents_2(avl);
    for (DataQueryKeyAvlNode2* node = first_dfs2(avl); node; node = next_dfs2(node)) {
        double value_entropy = 0;
        size_t total_value_rows = node->nr;
        calc_parents_1(node->subavl);
        for (DataQueryKeyAvlNode* n2 = first_dfs(node->subavl); n2; n2 = next_dfs(n2)) {
            double probability = (double)n2->nr / total_value_rows;
            value_entropy -= probability * log2(probability);
        }
        attribute_entropy += ((double)total_value_rows / table.nr_rows) * value_entropy;
    }

    /* "Calculate the entropy before the split" */

    double overall_entropy = 0;
    for (size_t i = 0; i < table.nr_rows; ++i) {
        DataQueryKey value_tgt = table.data[i*table.nr_columns + target_attribute];
        DataQueryKeyAvlNode* f2 = avl_find(avl_tgt, value_tgt, compare_dqks);
        if (!f2) {
            DataQueryKeyAvlNode* newf2 = avl_tgt_val_storage_p;
            avl_tgt = avl_insert(avl_tgt, value_tgt,
             avl_tgt_val_storage_p, compare_dqks);
             assert(newf2);
            ++avl_tgt_val_storage_p;
            f2 = newf2;
            f2->nr = 0;
            assert(avl_tgt);
        }
        ++f2->nr;
    }

    for (DataQueryKeyAvlNode* n2 = first_dfs(avl_tgt); n2; n2 = next_dfs(n2)) {
        double prob = (double) n2->nr / table.nr_rows;
        overall_entropy -= prob * log2(prob); 
    }

    free(avl_target_values_storage);
    free(avl_values_storage);
    free(avl_values_storage2);
    *result = overall_entropy - attribute_entropy;
    return 1;
}

static int get_best_attribute(size_t* result, DecisionTable table, int* attributes,
                                  size_t nr_attr, size_t tgt_attr) {

    double max_gain = 0;
    size_t best_attr_now = 0;

    for (size_t i = 0; i < table.nr_columns; ++i) {
        if (!attributes[i]) {
            double gain;
            int ss = information_gain(&gain, table, i, tgt_attr);
            if (!ss) {
                return 0;
            }
            if (gain > max_gain) {
                max_gain = gain;
                best_attr_now = i;
            }
        }
    }

    *result = best_attr_now;
    return 1;
}

TextTreeNode build_decision_tree(DecisionTable table, int* attributes,
                                  size_t nr_attr, size_t depth,
                                  size_t target_attribute,
                                  char* parent_text, TextTreeNode* parent) {
    TextTreeNode none;
    memset(&none, 0, sizeof(none));
    /* no data */

    if (table.nr_rows == 0) {
        return none;
    }
    
    /* all are the same */
    int same = 1;
    DataQueryKey t = table.data[0*table.nr_columns + target_attribute];
    for (size_t i = 1; i < table.nr_rows; ++i) {
        if (!data_queries_equal(table.data[i*table.nr_columns + target_attribute], t)) {
            same = 0;
            break;
        }
    }

    if (same) {
        TextTreeNode node;
        DataQueryKey dominantClass = table.data[0*table.nr_columns + target_attribute];
        DataQueryKey strCvt;
        int scss = convertToStr(&strCvt, dominantClass, 0);
        if (!scss) {
            return none;
        }

        node.node_text = malloc(strCvt.key.data.str.n+1);
        if (!node.node_text) {
            freeKey(&strCvt);
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
        int* modeArr = malloc(table.nr_rows * sizeof(int));
        if (!modeArr) {
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
            assert(idx*table.nr_columns+target_attribute < table.nr_columns * table.nr_rows);
            DataQueryKey nowKey = table.data[idx*table.nr_columns+target_attribute];
            ++count;
            

            modeArr[idx] = 1;
            ++all_count;

            

            while (idx != table.nr_rows) {
                if (!modeArr[idx] &&
                    data_queries_equal(nowKey, table.data[idx * table.nr_columns+target_attribute])) {
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
        int scss = convertToStr(&strCvt, dominantClass, 0);
        if (!scss) {
            return none;
        }

        node.node_text = malloc(strCvt.key.data.str.n+1);
        if (!node.node_text) {
            freeKey(&strCvt);
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
    int ss = get_best_attribute(&best_attr, table, attributes, nr_attr, target_attribute);

    if (!ss) {
        return none;
    }

    TextTreeNode newNode;

    newNode.node_text = malloc(strlen(table.titles[best_attr]) + 1);
    if (!newNode.node_text) {
        return none;
    }

    newNode.node_text[strlen(table.titles[best_attr])] = 0;
    strncpy(newNode.node_text, table.titles[best_attr], strlen(table.titles[best_attr]));

    DataQueryKey* bestAttrValues = malloc(table.nr_rows*sizeof(DataQueryKey));
    if (!bestAttrValues) {
        free(newNode.node_text);
        return none;
    }

    for (size_t i = 0; i < table.nr_rows; ++i) {
        bestAttrValues[i] = table.data[i*table.nr_columns+best_attr];
    }

    heapsort(bestAttrValues, sizeof(DataQueryKey), table.nr_rows, compare_dqks_ptr_);

     

    DataQueryKey currentValue = bestAttrValues[0];

    TextTreeNode* newNodeChildren = malloc(sizeof(TextTreeNode) * table.nr_rows);
    if (!newNodeChildren) {

        free(bestAttrValues);
free(newNode.node_text);
        return none;

    }
    size_t nrNewChildren = 0;

    for (size_t i = 0; i < table.nr_rows; ++i) {

        if (i == 0 || !data_queries_equal(currentValue, bestAttrValues[i])) {
            DecisionTable subdata = get_filtered_table(table, best_attr, currentValue);

            if (subdata.titles == NULL) {
                free(newNodeChildren);
                free(bestAttrValues);
                free(newNode.node_text);
                return none;
            }

            if (subdata.nr_rows == 0) {

                TextTreeNode* node = &newNodeChildren[nrNewChildren];

                /* the simplest way to calculate mode element */
                int* modeArr = malloc(table.nr_rows * sizeof(int));
                if (!modeArr) {
                    free(newNodeChildren);
                    free_filtered_table(subdata);
                        free(bestAttrValues);
                        free(newNode.node_text);
                    return none;
                }
                memset(modeArr, 0, sizeof(int) * table.nr_rows);

                DataQueryKey maxNowKey;
                size_t max_count = 0;
                size_t all_count = 0;
                while (all_count != table.nr_rows) {
                    size_t count = 0;
                    size_t idx = 0;
                    while (modeArr[idx]) ++idx;
                    DataQueryKey nowKey = table.data[idx*table.nr_columns+target_attribute];
                    ++count;
                    ++all_count;
                    modeArr[idx] = 1;
                    while (idx != table.nr_rows) {
                        if (!modeArr[idx] &&
                            data_queries_equal(nowKey, table.data[idx * table.nr_columns+target_attribute])) {
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
            int scss = convertToStr(&strCvt, dominantClass, 0);
            
            if (!scss) {
                free(newNodeChildren);
                free_filtered_table(subdata);
                    free(bestAttrValues);
                    free(newNode.node_text);
                return none;
            }

            node->node_text = malloc(strCvt.key.data.str.n+1);
            if (!node->node_text) {
                free(newNodeChildren);
                free_filtered_table(subdata);
                    free(bestAttrValues);
                    free(newNode.node_text);
                freeKey(&strCvt);
                return none;
            }

            strncpy(node->node_text, strCvt.key.data.str.ptr, strCvt.key.data.str.n);
            node->node_text[strCvt.key.data.str.n] = 0;

            freeKey(&strCvt);

            node->parent_text = parent_text;
            node->parent = parent;
            node->children = NULL;
            node->nr_children = 0;
            
            /*node*/

            ++nrNewChildren;

            } else {

                DataQueryKey newStrPtr;
                int scss = convertToStr(&newStrPtr, currentValue, 0);

                if (!scss) {
                    free(newNodeChildren);
                    free_filtered_table(subdata);
                    free(bestAttrValues);
                    free(newNode.node_text);
                    return none;
                }

                TextTreeNode* node = &newNodeChildren[nrNewChildren];
                attributes[best_attr] = 1;
                ++nr_attr;



                
                
                *node = build_decision_tree(subdata, attributes, nr_attr, depth-1, target_attribute, newStrPtr.key.data.str.ptr, &newNode);
                

                


                --nr_attr;
                attributes[best_attr] = 0;
                ++nrNewChildren;
            }
            currentValue = bestAttrValues[i];
            free_filtered_table(subdata);
        }
    }

    if (nrNewChildren) {
        newNode.children = newNodeChildren;
        newNode.nr_children = nrNewChildren;
    } else {
        free(newNodeChildren);
    }
    newNode.parent_text = parent_text;
    newNode.parent = parent;

    free(bestAttrValues);
    return newNode;
}






static int data_queries_equal(DataQueryKey left, DataQueryKey right) {
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
        return left.key.data.str.n == right.key.data.str.n &&
            strcmp(left.key.data.str.ptr, right.key.data.str.ptr) == 0;
    }

    if (left.type == DQKeyword) {
        return left.key.word == right.key.word;
    }

    if (left.type == DQNone) {
        return 1;
    }

    if (left.type == DQList) {
        if (left.key.data.list.n != right.key.data.list.n) {
            return 0;
        }

        for (size_t i = 0; i < left.key.data.list.n; ++i) {
            if (!data_queries_equal(left.key.data.list.root[i],right.key.data.list.root[i])) {
                return 0;
            }
        }
        return 1;
    }

    return 1;
}