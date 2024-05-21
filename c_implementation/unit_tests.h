#ifndef MINIML_UNIT_TESTS_H
#define MINIML_UNIT_TESTS_H
#include "string_tree.h"
#include <stdio.h>

void ut_read_csv_from(FILE *fp);

void parse_test();

void csv_test();

void xml_test();

void json_test();

#endif /* MINIML_UNIT_TESTS_H */