#include "decision_table_visualization.h"
#include <SFML/Graphics/PrimitiveType.h>
#include <SFML/Graphics/RenderWindow.h>

void table_visualization_draw(DecisionTableVisualization* tableViz, sfRenderWindow* window) {
    sfRenderWindow_drawPrimitives(window, tableViz->rectangleVertices, tableViz->nr_rectangleTriangles*3,
            sfTriangles, NULL);
    sfRenderWindow_drawPrimitives(window, tableViz->lines, tableViz->nr_lines*2, sfLines, NULL);
    for (size_t i = 0; i < tableViz->nr_texts; ++i) {
        sfRenderWindow_drawText(window, *(tableViz->texts+i), NULL);
    }
}

DecisionTableVisualization table_visualization_create(const DecisionTable* tree) {
    /*TODO*/
}