#include "unit_tests.h"
#include "csv_data_parser.h"
#include "json_parser.h"
#include "parser_utils.h"
#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>

void parse_test() {
  printf("--------------JSON TEST----------------\n");
  json_test();
  printf("--------------XML TEST----------------\n");
  xml_test();
  printf("--------------CSV TEST----------------\n");
  csv_test();
}

void ut_read_csv_from(FILE *fp) {
  char *str = get_file_contents(fp);
  if (!str)
    return;

  CsvData data = read_from_csv(str);
  if (!data.data) {
    free(data.data);
    free(str);
    fprintf(stderr, "Failed to read\n");
    return;
  }
  printf("Data capacity: %lu\nColumns: %lu\nRows: %lu\nContents:\n",
         data.data_capacity, data.nr_columns, data.nr_rows);

  for (int i = 0; i < data.nr_rows; ++i) {
    for (int j = 0; j < data.nr_columns; ++j) {
      printf("[%s] ", data.data[i * data.nr_columns + j]);
    }
    printf("\n");
  }
  for (int i = 0; i < data.data_capacity; ++i) {
    free(data.data[i]);
  }
  free(data.data);
  free(str);
}

void csv_test() {
  FILE *fp = fopen("data/data1.csv", "r");
  if (!fp)
    return;
  ut_read_csv_from(fp);
  fclose(fp);
}

extern const char *const xml_contents;

void xml_test() {
  FILE *fp = fopen("data/example.xml", "r");
  if (!fp)
    return;
  char *str = get_file_contents(fp);
  fclose(fp);
  if (!str) {
    return;
  }

  StringTreeNode xml = read_from_xml(str);
  free(str);
  printStringTree(&xml, 0);
  free_string_tree_children(xml);
  return;
}

void json_test() {
  FILE *fp = fopen("data/example.json", "r");
  if (!fp)
    return;
  char *str = get_file_contents(fp);
  fclose(fp);
  if (!str) {
    return;
  }

  StringTreeNode json = read_from_json(str);
  free(str);
  printStringTree(&json, 0);
  free_string_tree_children(json);
  return;
}

const char *const xml_contents =
    "<breakfast_menu>"
    "<food>"
    "<name>Belgian Waffles</name>"
    "<price>$5.95</price>"
    "<description>"
    "Two of our famous Belgian Waffles with plenty of real maple syrup"
    "</description>"
    "<calories>650</calories>"
    "</food>"
    "<food>"
    "<name>Strawberry Belgian Waffles</name>"
    "<price>$7.95</price>"
    "<description>"
    "Light Belgian waffles covered with strawberries and whipped cream"
    "</description>"
    "<calories>900</calories>"
    "</food>"
    "<food>"
    "<name>Berry-Berry Belgian Waffles</name>"
    "<price>$8.95</price>"
    "<description>"
    "Light Belgian waffles covered with an assortment of fresh berries and "
    "whipped cream"
    "</description>"
    "<calories>900</calories>"
    "</food>"
    "<food>"
    "<name>French Toast</name>"
    "<price>$4.50</price>"
    "<description>"
    "Thick slices made from our homemade sourdough bread"
    "</description>"
    "<calories>600</calories>"
    "</food>"
    "<food>"
    "<name>Homestyle Breakfast</name>"
    "<price>$6.95</price>"
    "<description>"
    "Two eggs, bacon or sausage, toast, and our ever-popular hash browns"
    "</description>"
    "<calories>950</calories>"
    "</food>"
    "</breakfast_menu>";