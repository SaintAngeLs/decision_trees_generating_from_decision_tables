#include "data_format_conversion.h"
#include "data_query.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void free_data_table(DataQueryKey* data, size_t width, size_t height) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            freeKey(data + (i * width + j));
        }
    }
    free(data);
}

DecisionTable create_decision_tree_from_csv(CsvData* csv, DataQueryType* types) {
    DecisionTable dectab;
    memset(&dectab, 0, sizeof(dectab));
    
    char** titles = malloc(sizeof(char*) * csv->nr_columns);
    if (!titles) {
        return dectab;
    }

    DataQueryKey* newData = malloc((csv->nr_rows-1)*csv->nr_columns * sizeof(DataQueryKey));
    if (!newData) {
        free(titles);
        return dectab;
    }

    memset(titles, 0,sizeof(char*) * csv->nr_columns);

    for (size_t i = 0; i < csv->nr_columns; ++i) {
        titles[i] = malloc(strlen(csv->data[i])+1);

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
                newData[(i-1)*csv->nr_columns+j].key.data.integ =
                    strtol(csv->data[i*csv->nr_columns+j], NULL, 10);
                newData[(i-1)*csv->nr_columns+j].type = DQInt;
            }
            else if (types[j] == DQReal) {
                newData[(i-1)*csv->nr_columns+j].key.data.real = 
                    strtod(csv->data[i*csv->nr_columns+j], NULL);
                newData[(i-1)*csv->nr_columns+j].type = DQReal;
            }
            else { /* str */
                
                newData[(i-1)*csv->nr_columns+j].key.data.str.ptr =
                    malloc(strlen(csv->data[i*csv->nr_columns+j])+1);

                if (!newData[(i-1)*csv->nr_columns+j].key.data.str.ptr) {
                    for (size_t ii = 0; ii < csv->nr_columns; ++ii) {
                        free(titles[ii]);
                    }  
                    free(titles);
                    free_data_table(newData, csv->nr_columns, csv->nr_rows-1);
                    free(newData);
                    return dectab;
                }

                newData[(i-1)*csv->nr_columns+j].key.data.str.ptr[strlen(csv->data[i*csv->nr_columns+j])] = 0;
                strncpy(newData[(i-1)*csv->nr_columns+j].key.data.str.ptr, csv->data[i*csv->nr_columns+j],
                    strlen(csv->data[i*csv->nr_columns+j]));
                newData[(i-1)*csv->nr_columns+j].type = DQString;
            }
        }
    }

    dectab.data = newData;
    dectab.titles = titles;
    dectab.nr_columns = csv->nr_columns;
    dectab.nr_rows = csv->nr_rows-1;
    return dectab;
}

DecisionTable create_decision_tree_from_parsed_tree(StringTreeNode tree,
        DataQueryKey** instrPtr,
        size_t* instr_size_ptr,
        size_t nr_columns) {

    DecisionTable dectab;
    memset(&dectab, 0, sizeof(dectab));
    
    size_t n = tree.nr_children;
    if (n == 0) {
        return dectab;
    }

    assert(tree.children);

    DataQueryKey* newData = malloc(n*nr_columns * sizeof(DataQueryKey));
    if (!newData) {
        return dectab;
    }

    memset(newData, 0, sizeof(DataQueryKey));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < nr_columns; ++j) {
            newData[i*nr_columns+j] = miniml_data_query(instrPtr[j], instr_size_ptr[j], tree.children[i]);
            if (newData[i*nr_columns+j].type == DQNone) {
                free_data_table(newData, nr_columns, n);
                free(newData);
                return dectab;
            }
        }
    }

    dectab.data = newData;
    dectab.nr_columns = nr_columns;
    dectab.nr_rows = n;
    return dectab;
}