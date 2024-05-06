#ifndef MINIML_DATA_FORMAT_CONVERSION_H
#define MINIML_DATA_FORMAT_CONVERSION_H
#include "data_query.h"
#include "parser_utils.h"
#include "csv_data_parser.h"
#include "string_tree.h"

typedef struct {
    char** titles;
    size_t nr_columns;
    size_t nr_rows;
    DataQueryKey* data;
} DecisionTable;

DecisionTable create_decision_tree_from_csv(CsvData* csv, DataQueryType* types);

/* seletion == NULL => selects all with property */
/* order matters */
/* ADD TITLES */
DecisionTable create_decision_tree_from_parsed_tree(StringTreeNode tree,
        DataQueryKey** instrPtr,
        size_t* instr_size_ptr,
        size_t nr_columns);

#endif /* MINIML_DATA_FORMAT_CONVERSION_H */