#ifndef MINIML_DECISION_TABLE_H
#define MINIML_DECISION_TABLE_H
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

enum DataType {
    DataString,
    DataInteger,
    DataReal,
    DataNrTypes
};

union Data {
    /* Unicode string */
    uint_least32_t* str;
    double fl;
    intmax_t intg;
};

typedef struct {
    enum DataType* types;
    uint_least32_t** titles;
    size_t nr_columns;
    size_t nr_rows;
    union Data* data;
} DecisionTable;

#endif /* MINIML_DECISION_TABLE_H */