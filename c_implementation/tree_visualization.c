#include "tree_visualization.h"
#include "text_tree.h"
#include <SFML/Graphics/CircleShape.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/PrimitiveType.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/Vertex.h>
#include <SFML/System/Vector2.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void tree_visualization_draw(const TextTreeVisualization* tree, sfRenderWindow* window) {
    /*draw pointers*/
    sfRenderWindow_drawPrimitives(window, tree->pointers,
        tree->nr_pointers_lines*2,
            sfLines, NULL);
    /*draw nodes*/
    for (size_t i = 0; i < tree->nr_nodes; ++i) {
        sfRenderWindow_drawCircleShape(window, tree->nodes[i], NULL);
    }
    /*draw texts*/
    for (size_t i = 0; i < tree->nr_texts; ++i) {
        sfRenderWindow_drawText(window, *(tree->texts + i), NULL);
    }
}

static size_t calc_width(TextTreeNode* root, size_t lvl) {
    root->lvl = lvl;

    if (root->nr_children == 0) {
        root->width = 1;
        return 1;
    }

    size_t sum = 0;
    for (size_t i = 0; i < root->nr_children; ++i) {
        sum += calc_width(&root->children[i], lvl+1);
    }

    root->width = sum;
    return sum;
}

static TextTreeNode* first_dfs(TextTreeNode* root) {
    return root;
}

static TextTreeNode* next_dfs(TextTreeNode* root) {
    assert(root);

    if (root->nr_children != 0) {
        return root->children;
    }

    TextTreeNode* parent = root->parent;

    if (!parent) {
        return NULL;
    }

    while (parent->nr_children == root->parent_index + 1) {
        root = parent;
        parent = root->parent;

        if (!parent) {
            return NULL;
        }
    }

    return parent->children + root->parent_index + 1;
}

static void calc_parent_indices(TextTreeNode* root) {
    for (size_t i = 0; i < root->nr_children; ++i) {
        root->children[i].parent_index = i;
        
        /* correct parents */
        root->children[i].parent = root;

        calc_parent_indices(&root->children[i]);
    }
}

static size_t calc_height(TextTreeNode* root) {
    if (root->nr_children == 0) {
        return 1;
    }

    size_t max_ch_h = 1;
    for (size_t i = 0; i < root->nr_children; ++i) {
        size_t m = calc_height(&root->children[i]);
        if (m > max_ch_h) {
            max_ch_h = m;
        }
    }

    return 1 + max_ch_h;
}

static TextTreeNode* calc_leafs(TextTreeNode* root) {
    TextTreeNode* p = first_dfs(root);
    TextTreeNode sentinelLeaf;
    TextTreeNode* currLeaf = &sentinelLeaf;
    size_t leaf_index = 0;
    currLeaf->leaf_next = NULL;
    while (p) {
        if (p->nr_children == 0) {
            currLeaf->leaf_next = p;
            currLeaf = p;
            currLeaf->left = leaf_index;
            currLeaf->right = leaf_index;
            ++leaf_index;
            currLeaf->leaf_next = NULL;
        }
        p = next_dfs(p);
    }
    return sentinelLeaf.leaf_next;
}

static void calc_ranges(TextTreeNode* root) {
    if (root->nr_children==0) {
        return;
    }

    for (size_t i = 0; i < root->nr_children; ++i) {
        calc_ranges(&root->children[i]);
    }

    size_t left = root->children[0].left;
    size_t right = root->children[root->nr_children-1].right;
    root->left = left;
    root->right = right;
}

 const float node_width = 200;
    const float node_height = 100;
    const unsigned char_size = 25;
    const float node_space_width = 300;
    const float node_space_height = 200;
    const float node_x_inside = 50;
    const float node_y_inside = 50;

    const float up_ptr_x = node_x_inside + node_width / 2;
    const float up_ptr_y = node_y_inside;
    const float down_ptr_x = up_ptr_x;
    const float down_ptr_y = node_y_inside + node_height;

    const float text_x_inside = node_x_inside + node_width / 2;
    const float text_y_inside = node_y_inside + node_height / 2;



static void calc_positions(TextTreeNode* root) {
    float half = (root->right + root->left)/2.f;
    float x_space = half * node_space_width;
    float y_space = root->lvl * node_space_height;

    root->x = x_space + node_x_inside;
    root->y = y_space + node_y_inside;

    root->text_x = root->x + node_width / 2;
    root->text_y = root->y + node_height / 2;

    for (size_t i = 0; i < root->nr_children; ++i) {
        calc_positions(&root->children[i]);
    }
}

static void calc_ptr_positions(TextTreeNode* root) {
    if (root->parent) {
        float x = root->x;
        float y = root->y;
        float px = root->parent->x;
        float py = root->parent->y;
        
        float ptrx = x + node_width / 2;
        float ptry = y;

        float ptxpx = px + node_width / 2;
        float ptxpy = py + node_height;

        float tx = (ptrx+ptxpx)/2;
        float ty = (ptry+ptxpy)/2;

        root->ptr_px = ptxpx;
        root->ptr_py = ptxpy;
        root->ptr_x = ptrx;
        root->ptr_y = ptry;
        root->ptr_text_x = tx;
        root->ptr_text_y = ty;
    }

    for (size_t i = 0; i < root->nr_children; ++i) {
        calc_ptr_positions(&root->children[i]);
    }
}

static size_t calc_ptr_count(TextTreeNode* tree) {
    if (tree->nr_children == 0) {
        return 0;
    }
    size_t n = 0;
    for (size_t i = 0; i < tree->nr_children; ++i) {
        n += calc_ptr_count(&tree->children[i]);
    }
    return n + tree->nr_children;
}

static size_t calc_count(TextTreeNode* tree) {
    if (tree->nr_children == 0) {
        return 1;
    }

    size_t n = 0;
    for (size_t i = 0; i < tree->nr_children; ++i) {
        n += calc_count(&tree->children[i]);
    }

    return 1 + n;
}

void free_visualization_tree(TextTreeVisualization* tree) {
    for (size_t i = 0; i < tree->nr_texts; ++i) {
        sfText_destroy(tree->texts[i]);
    }
    for (size_t i = 0; i < tree->nr_nodes; ++i) {
        sfCircleShape_destroy(tree->nodes[i]);
    }
    free(tree->nodes);
    free(tree->pointers);
    free(tree->texts);
}

TextTreeVisualization tree_visualization_create(TextTreeNode* tree, sfFont* font) {
    TextTreeVisualization result;
    memset(&result, 0, sizeof(result));

    /* calculate widths */
    (void)calc_width(tree, 0);
    calc_parent_indices(tree);
    (void)calc_leafs(tree);
    calc_ranges(tree);

    size_t width = tree->right + 1 - tree->left;
    size_t height = calc_height(tree);

    calc_positions(tree);
    calc_ptr_positions(tree);

    size_t n = calc_count(tree);
    size_t ptrn = calc_ptr_count(tree);

    size_t ptr_nvcs = ptrn * 2;
    


    sfCircleShape** cspp = calloc(n, sizeof(sfCircleShape*));
    if (!cspp) {
        return result;
    }
    int success = 1;
    for (size_t i = 0; i < n; ++i) {
        cspp[i] = sfCircleShape_create();
        if (!cspp[i]) {
            success = 0;
            break;
        }
    }
    if (!success) {
        for (size_t i = 0; i < n; ++i) {
            sfCircleShape_destroy(cspp[i]);
        }
        free(cspp);
        return result;
    }

    for (size_t i = 0; i < n; ++i) {
        sfCircleShape_setFillColor(cspp[i], (sfColor){200, 200, 255, 255});
        sfCircleShape_setOutlineThickness(cspp[i], 2);
        sfCircleShape_setOutlineColor(cspp[i], (sfColor){0, 0, 0, 255});
        sfCircleShape_setRadius(cspp[i], node_height/2.f);
        sfCircleShape_setScale(cspp[i], (sfVector2f){node_width/node_height, 1});
    }



    sfText** txtpp = calloc(n + ptrn,sizeof(sfText*));
    if (!txtpp) {
        for (size_t i = 0; i < n; ++i) {
            sfCircleShape_destroy(cspp[i]);
        }
        free(cspp);
        return result;
    }

    for (size_t i = 0; i < n + ptrn; ++i) {
        txtpp[i] = sfText_create();
        if (!txtpp[i]) {
            success = 0;
            break;
        }
    }
    if (!success) {
        for (size_t i = 0; i < n; ++i) {
            sfCircleShape_destroy(cspp[i]);
        }
        free(cspp);
        for (size_t i = 0; i < (n+ptrn); ++i) {
            sfText_destroy(txtpp[i]);
        }
        free(txtpp);
        return result;
    }

    sfText** ptr_txtpp = txtpp + n;
    sfColor black;
    black.r = black.g = black.b = 0;
    black.a = 255;

    for (size_t i = 0; i < n; ++i) {
        sfText_setFont(txtpp[i], font);
        sfText_setFillColor(txtpp[i], black);
        sfText_setCharacterSize(txtpp[i], char_size);
    }

    for (size_t i = 0; i < ptrn; ++i) {
        sfText_setFont(ptr_txtpp[i], font);
        sfText_setFillColor(ptr_txtpp[i], black);
        sfText_setCharacterSize(ptr_txtpp[i], char_size);
    }



    size_t i = 0;
    for (TextTreeNode* node = first_dfs(tree); node; ++i, node = next_dfs(node)) {
        sfText_setString(txtpp[i], node->node_text);
        sfText_setPosition(txtpp[i], (sfVector2f){node->text_x, node->text_y});
        sfFloatRect fr = sfText_getGlobalBounds(txtpp[i]);
        sfText_setOrigin(txtpp[i], (sfVector2f){fr.width/2,fr.height/2});
        sfCircleShape_setPosition(cspp[i], (sfVector2f){node->x, node->y});
    }

    i = 0;
    for (TextTreeNode* node = next_dfs(first_dfs(tree)); node; ++i, node = next_dfs(node)) {
        sfText_setString(ptr_txtpp[i], node->parent_text);
        sfText_setPosition(ptr_txtpp[i], (sfVector2f){node->ptr_text_x, node->ptr_text_y});
        sfFloatRect fr = sfText_getGlobalBounds(ptr_txtpp[i]);
        sfText_setOrigin(ptr_txtpp[i], (sfVector2f){fr.width/2,fr.height/2});
    }

    /* pointers */

    sfVertex* vcs = malloc(sizeof(sfVertex) * ptrn * 2);
    if (!vcs) {
        for (size_t i = 0; i < n; ++i) {
            sfCircleShape_destroy(cspp[i]);
        }
        free(cspp);
        for (size_t i = 0; i < (n+ptrn); ++i) {
            sfText_destroy(txtpp[i]);
        }
        free(txtpp);
        return result;
    }
    memset(vcs, 0, sizeof(sfVertex)*ptrn*2);

    i = 0;
    for (TextTreeNode* node = next_dfs(first_dfs(tree)); node; ++i, node = next_dfs(node)) {
        vcs[i*2].color = black;
        vcs[i*2].position = (sfVector2f){ node->ptr_px, node->ptr_py };
        vcs[i*2+1].color = black;
        vcs[i*2+1].position = (sfVector2f){ node->ptr_x, node->ptr_y };
    }

    result.nodes = cspp;
    result.pointers = vcs;
    result.texts = txtpp;
    result.nr_texts = n+ptrn;
    result.nr_nodes = n;
    result.nr_pointers_lines = ptrn;
    return result;
}