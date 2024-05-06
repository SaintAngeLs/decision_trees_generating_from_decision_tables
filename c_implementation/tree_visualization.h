#ifndef MINIML_TREE_VISUALIZATION_H
#define MINIML_TREE_VISUALIZATION_H
#include "text_tree.h"
#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/Vertex.h>
#include <stddef.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/CircleShape.h>

typedef struct _TextTreeVisualization {
    sfText** texts;
    sfCircleShape** nodes;
    sfVertex* pointers;

    size_t nr_texts;
    size_t nr_nodes;
    size_t nr_pointers_lines;
} TextTreeVisualization;

void tree_visualization_draw(const TextTreeVisualization* tree, sfRenderWindow* window);

/* TODO: unit test */
TextTreeVisualization tree_visualization_create(TextTreeNode* tree, sfFont* font);

#endif /* MINIML_TREE_VISUALIZATION_H */