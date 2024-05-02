#include "data_format_conversion.h"
#include "decision_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/*without 0*/
size_t strlen32(uint_least32_t* str) {
    size_t n = 0;
    while (*str != 0) {
        ++str;
        ++n;
    }
    return n;
}

int is_string_float_ascii(char* str) {
    char dummy;
    double fl;
    if (sscanf(str, "%lf%c", &fl, &dummy) != 1) {
        return 0;
    }
    return 1;
}

/* -1 if error */
int is_string_float(uint_least32_t* str) {
    size_t n = strlen32(str);
    char* temp = malloc(n+1);
    if (!temp) {
        return -1;
    }
    for (size_t i = 0; i < n; ++i) {
        temp[i] = (char)str[i];
    }
    temp[n] = 0;

    for (size_t i = 0; i < n; ++i) {
        if (temp[i] != str[i]) {
            free(temp);
            return 0;
        }
    }

    int result = is_string_float_ascii(temp);
    free(temp);
    return result;
}

int is_string_int_ascii(char* str) {
    char dummy;
    intmax_t integ;
    if (sscanf(str, "%ld%c", &integ, &dummy) != 1) {
        return 0;
    }
    return 1;
}

/* -1 if error */
int is_string_int(uint_least32_t* str) {
    size_t n = strlen32(str);
    char* temp = malloc(n+1);
    if (!temp) {
        return -1;
    }
    for (size_t i = 0; i < n; ++i) {
        temp[i] = (char)str[i];
    }
    temp[n] = 0;

    for (size_t i = 0; i < n; ++i) {
        if (temp[i] != str[i]) {
            free(temp);
            return 0;
        }
    }

    int result = is_string_int_ascii(temp);
    free(temp);
    return result;
}

enum DataType get_data_type_from_data_array(uint_least32_t** arr, size_t n, size_t distance) {
    /* check if all are numbers and some of them are with floating point */
    int lvl = 0;
    for (size_t i = 0; i < n; ++i) {
        uint_least32_t** str = arr + i * distance;
        if (1 == is_string_float(*str)) {
            lvl = 1;
        } else if (1 != is_string_int(*str)) {
            lvl = -1;
            break;
        }
    }
    if (lvl == -1) {
        return DataString;
    }
    if (lvl == 0) {
        return DataInteger;
    }
    if (lvl == 1) {
        return DataReal;
    }

    return DataNrTypes;
}

enum DataType get_data_type_from_data_array_ascii(char** arr, size_t n, size_t distance) {
    /* check if all are numbers and some of them are with floating point */
    int lvl = 0;
    for (size_t i = 0; i < n; ++i) {
        char** str = arr + i * distance;
        if (1 == is_string_float_ascii(*str)) {
            lvl = 1;
        } else if (1 != is_string_float_ascii(*str)) {
            lvl = -1;
            break;
        }
    }
    if (lvl == -1) {
        return DataString;
    }
    if (lvl == 0) {
        return DataInteger;
    }
    if (lvl == 1) {
        return DataReal;
    }

    return DataNrTypes;
}

uint_least32_t* get_unicode_from_ascii(char* ascii) {
    size_t n = strlen(ascii);
    uint_least32_t* unic = malloc(sizeof(uint_least32_t) * (n+1));
    if (!unic) {
        return NULL;
    }
    for (size_t i = 0; i < n; ++i) {
        unic[i] = (uint_least32_t)ascii[i];
    }
    unic[n] = 0;
    return unic;
}

void free_data_table(enum DataType* types, union Data* data, size_t width, size_t height) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (types[j] == DataString) {
                free(data[i*width+j].str);
            }
        }
    }
    free(data);
    free(types);
}

DecisionTable create_decision_tree_from_csv(CsvData* csv) {
    DecisionTable dectab;
    memset(&dectab, 0, sizeof(dectab));
    
    enum DataType* types = malloc(csv->nr_columns * sizeof(enum DataType));
    if (!types) {
        return dectab;
    }

    uint_least32_t** titles = malloc(sizeof(uint_least32_t*) * csv->nr_columns);
    if (!titles) {
        free(types);
        return dectab;
    }

    union Data* newData = malloc((csv->nr_rows-1)*csv->nr_columns * sizeof(union Data));
    if (!newData) {
        free(types);
        free(titles);
        return dectab;
    }

    for (size_t i = 0; i < csv->nr_columns; ++i) {
        enum DataType dataType = get_data_type_from_data_array_ascii((csv->data+csv->nr_columns) + i,
            csv->nr_rows, csv->nr_columns);
        types[i] = dataType;
    }

    memset(titles, 0,sizeof(uint_least32_t*) * csv->nr_columns);

    for (size_t i = 0; i < csv->nr_columns; ++i) {
        titles[i] = get_unicode_from_ascii(csv->data[i]);
        if (!titles[i]) {
            for (size_t ii = 0; ii < csv->nr_columns; ++ii) {
                free(titles[ii]);
            } 
            free(types);
            free(titles);
            return dectab;
        }
    }

    memset(newData, 0, sizeof(union Data));
    for (size_t i = 1; i < csv->nr_rows; ++i) {
        for (size_t j = 0; j < csv->nr_columns; ++j) {
            if (types[j] == DataInteger) {
                newData[(i-1)*csv->nr_columns+j].intg = strtol(csv->data[i*csv->nr_columns+j], NULL, 10);
            }
            else if (types[j] == DataReal) {
                newData[(i-1)*csv->nr_columns+j].fl = strtod(csv->data[i*csv->nr_columns+j], NULL);
            }
            else {
                newData[(i-1)*csv->nr_columns+j].str = get_unicode_from_ascii(csv->data[i*csv->nr_columns+j]);
                if (!newData[(i-1)*csv->nr_columns+j].str) {
                    for (size_t ii = 0; ii < csv->nr_columns; ++ii) {
                        free(titles[ii]);
                    }  
                    free(titles);
                    free_data_table(types, newData, csv->nr_columns, csv->nr_rows-1);
                    return dectab;
                }
            }
        }
    }

    dectab.data = newData;
    dectab.titles = titles;
    dectab.types = types;
    dectab.nr_columns = csv->nr_columns;
    dectab.nr_rows = csv->nr_rows-1;
    return dectab;
}

DecisionTable create_decision_tree_from_parsed_tree(StringTreeNode* tree) {
    assert(tree);
    DecisionTable dectab;
    memset(&dectab, 0, sizeof(dectab));
    
    size_t n = tree->nr_children;
    if (n == 0) {
        return dectab;
    }

    assert(tree->children);

    /* validation */
    size_t mtemp = tree->children[0].nr_children;
    if (mtemp == 0) {
        return dectab; 
    }

    for (size_t i = 1; i < n; ++i) {
        if (tree->children[i].nr_children != mtemp) {
            return dectab;
        }
        for (size_t j = 0; j < mtemp; ++j) {
            if (strcmp(tree->children[i].children[j].name,
                       tree->children[0].children[j].name) != 0) {
                return dectab;
            }
        }
    }

    /* what columns are useful */
    size_t m = 0;
    int* useful_columns = malloc(mtemp*sizeof(int*));
    if (!useful_columns) {
        return dectab;
    }
    memset(useful_columns, 0, mtemp*sizeof(int*));

    for (size_t i = 0; i < mtemp; ++i) {
        if (tree->children[0].children[i].data &&
            strlen(tree->children[0].children[i].data) != 0) {
                m++;
                useful_columns[i] = 1;
        }
    }

    if (m == 0) {
        free(useful_columns);
        return dectab;
    }

    /* n rows */
    /* m columns */

    enum DataType* types = malloc(m * sizeof(enum DataType));
    if (!types) {
        free(useful_columns);
        return dectab;
    }

    uint_least32_t** titles = malloc(sizeof(uint_least32_t*) * m);
    if (!titles) {
        free(useful_columns);
        free(types);
        return dectab;
    }
    memset(titles, 0, sizeof(uint_least32_t*) * m);

    union Data* newData = malloc(n*m * sizeof(union Data));
    if (!newData) {
        free(useful_columns);
        free(types);
        free(titles);
        return dectab;
    }

    size_t k = 0;
    for (size_t i = 0; i < mtemp; ++i) {
        if (useful_columns[i]) {
            enum DataType dataType;
            char** temp = malloc(sizeof(char*) * n);
            if (!temp) {
                free(useful_columns);
                free(types);
                free(titles);
                return dectab;
            }
            for (size_t j = 0; j < n; ++j) {
                temp[j] = tree->children[j].children[k].data;
            }
            dataType = get_data_type_from_data_array_ascii(temp, n, 1);
            free(temp);
            types[k] = dataType;
            ++k;
        }
    }

    k = 0;
    for (size_t i = 0; i < mtemp; ++i) {
        if (useful_columns[i]) {
            titles[i] = get_unicode_from_ascii(tree->children[0].children[k].name);
            if (!titles[i]) {
                for (size_t ii = 0; ii < m; ++ii) {
                    free(titles[ii]);
                }
                free(useful_columns);
                free(types);
                free(titles);
                return dectab;
            }
            ++k;
        }
    }

    memset(newData, 0, sizeof(union Data));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (types[j] == DataInteger) {
                newData[(i)*m+j].intg = strtol(tree->children[i].children[j].data, NULL, 10);
            }
            else if (types[j] == DataReal) {
                newData[(i)*m+j].fl = strtod(tree->children[i].children[j].data, NULL);
            }
            else {
                newData[(i)*m+j].str = get_unicode_from_ascii(tree->children[i].children[j].data);
                if (!newData[(i)*m+j].str) {
                    for (size_t ii = 0; ii < m; ++ii) {
                        free(titles[ii]);
                    }   
                    free(titles);
                    free(useful_columns);
                    free_data_table(types, newData, m, n);
                    return dectab;
                }
            }
        }
    }

    dectab.data = newData;
    dectab.titles = titles;
    dectab.types = types;
    dectab.nr_columns = m;
    dectab.nr_rows = n;
    return dectab;
}