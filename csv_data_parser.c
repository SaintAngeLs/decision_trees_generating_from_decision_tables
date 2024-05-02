#include "csv_data_parser.h"
#include <stdlib.h>
#include <string.h>

int is_row_separator(char c) {
    return c == ';' || c == '\n' || c == '\r' || c == '\0';
}

CsvData read_from_csv(char* str) {
    CsvData data;
    memset(&data, 0, sizeof(data));

    while (is_row_separator(*str)) {
        ++str;
    }

    data.nr_columns = 1;
    data.nr_rows = 1;

    /* width */
    char* str1 = str;
    while (!is_row_separator(*str1)) {
        ++str1;
    }

    for (char* c = str; c != str1; ++c) {
        if (*c == ',') {
            data.nr_columns++;
        }
    }

    /* height */
    size_t tmp_nr = 1;
    int was_sep = 0;
    for (char* c = str; *c != '\0'; ++c) {
        if (was_sep) {
            while (is_row_separator(*c) && *c != '\0') {
                ++c;
            }
            if (*c == '\0') {
                break;
            }
            was_sep = 0;
        }
        if (*c == ',') {
            ++tmp_nr;
        }
        if (is_row_separator(*c)) {
            if (tmp_nr != data.nr_columns) {
                memset(&data, 0, sizeof(data));
                return data;
            }
            data.nr_rows++;
            tmp_nr = 1;
            was_sep = 1;
        }
    }

    data.data_capacity = data.nr_rows * data.nr_columns;
    data.data = malloc(data.data_capacity * sizeof(char*));

    if (!data.data) {
        memset(&data, 0, sizeof(data));
        return data;
    }

    memset(data.data, 0, data.data_capacity* sizeof(char*));

    char *a, *b;
    a = str;
    b = str;

    while (*b != ',' && !is_row_separator(*b)) {
        ++b;
    }
    data.data[0] = malloc(b-a+1);
    if (!data.data[0]) {
        free(data.data);
        memset(&data, 0, sizeof(data));
        return data;
    }
    strncpy(data.data[0], a, b-a);
    data.data[0][b-a] = 0;
    
    size_t nr_toks = 1;

    while (*b != '\0') {
        if (*b == ',' && is_row_separator(*(b+1))) {
            a = b + 1;
            b = b + 1;
        } else {
            ++b;
            while (is_row_separator(*b) && *b != 0) {
                ++b;
            }
            a = b;
            while (*b != ',' && !is_row_separator(*b)) {
                ++b;
            }
        }

        data.data[nr_toks] = malloc(b-a+1);
        if (!data.data[nr_toks]) {
            for (int i = 0; i < nr_toks; ++i) {
                free(data.data[i]);
            }
            free(data.data);
            memset(&data, 0, sizeof(data));
            return data;
        }
        strncpy(data.data[nr_toks], a, b-a);
        data.data[nr_toks][b-a] = 0;
        ++nr_toks;
    }

    data.nr_rows = nr_toks / data.nr_columns;
    return data;
}