#ifndef MINIML_TREE_VISUALIZATION_H
#define MINIML_TREE_VISUALIZATION_H
#include "text_tree.h"
#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/Vertex.h>
#include <stddef.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/Font.h>

typedef struct _TextTreeVisualization {
    sfText** texts;
    size_t nr_texts;
    sfVertex* nodes;
    size_t nr_node_triangles;
    sfVertex* pointers;
    size_t nr_pointer_triangles;
} TextTreeVisualization;

void tree_visualization_draw(const TextTreeVisualization* tree, sfRenderWindow* window);

TextTreeVisualization tree_visualization_create(const TextTreeNode* tree);

#endif /* MINIML_TREE_VISUALIZATION_H */