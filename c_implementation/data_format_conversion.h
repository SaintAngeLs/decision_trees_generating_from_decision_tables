#ifndef MINIML_DATA_FORMAT_CONVERSION_H
#define MINIML_DATA_FORMAT_CONVERSION_H
#include "csv_data_parser.h"
#include "data_query.h"
#include "parser_utils.h"
#include "string_tree.h"

/* if no titles, that's invalid */
typedef struct {
  char **titles;
  size_t nr_columns;
  size_t nr_rows;
  DataQueryKey *data;
} DecisionTable;

void print_decision_table(DecisionTable table);

void free_decision_table(DecisionTable table, int with_titles);

DecisionTable create_decision_table_from_csv(CsvData *csv,
                                             DataQueryType *types);

/* seletion == NULL => selects all with property */
/* order matters */
/* titles are copied not deeply */
DecisionTable create_decision_table_from_parsed_tree(StringTreeNode tree,
                                                     char **titles,
                                                     DataQueryKey **instrPtr,
                                                     size_t *instr_size_ptr,
                                                     size_t nr_columns);

/* no checks */
void convert_column_to_real(DecisionTable table, size_t column);

#endif /* MINIML_DATA_FORMAT_CONVERSION_H */