#ifndef MINIML_DATA_PARSER_H
#define MINIML_DATA_PARSER_H
#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t nr_columns;
    size_t nr_rows;
    char** data;
    size_t data_capacity; /* nr of char* allocated */
} CsvData;

CsvData read_from_csv(char* str);

#endif /* MINIML_DATA_PARSER_H */