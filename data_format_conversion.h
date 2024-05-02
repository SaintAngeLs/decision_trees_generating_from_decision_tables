#ifndef MINIML_DATA_FORMAT_CONVERSION_H
#define MINIML_DATA_FORMAT_CONVERSION_H
#include "decision_table.h"
#include "parser_utils.h"
#include "csv_data_parser.h"
#include "string_tree.h"

DecisionTable create_decision_tree_from_csv(CsvData* csv);

/* seletion == NULL => selects all with property */
/* order matters */
DecisionTable create_decision_tree_from_parsed_tree(StringTreeNode* tree);

#endif /* MINIML_DATA_FORMAT_CONVERSION_H */