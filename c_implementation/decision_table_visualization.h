#ifndef MINIML_DECISION_TABLE_VISUALIZATION_H
#define MINIML_DECISION_TABLE_VISUALIZATION_H
#include "decision_table.h"
#include "tree_visualization.h"

typedef struct {
    sfVertex* lines;
    size_t nr_lines;
    sfText** texts;
    size_t nr_texts;
    sfVertex* rectangleVertices;
    size_t nr_rectangleTriangles;
} DecisionTableVisualization;

void table_visualization_draw(DecisionTableVisualization* tableViz, sfRenderWindow* window);

DecisionTableVisualization table_visualization_create(const DecisionTable* tree);

#endif /* MINIML_DECISION_TABLE_VISUALIZATION_H */