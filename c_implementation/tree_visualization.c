#include "tree_visualization.h"
#include <SFML/Graphics/PrimitiveType.h>
#include <SFML/Graphics/RenderWindow.h>

void tree_visualization_draw(const TextTreeVisualization* tree,sfRenderWindow* window) {
    /*draw pointers*/
    sfRenderWindow_drawPrimitives(window, tree->pointers,
        tree->nr_pointer_triangles*3,
            sfTriangles, NULL);
    /*draw nodes*/
    sfRenderWindow_drawPrimitives(window, tree->nodes,
        tree->nr_node_triangles*3,
            sfTriangles, NULL);
    /*draw texts*/
    for (size_t i = 0; i < tree->nr_texts; ++i) {
        sfRenderWindow_drawText(window, *(tree->texts + i), NULL);
    }
}

TextTreeVisualization tree_visualization_create(const TextTreeNode* tree) {
    /* TODO */
}