#include "build_decision_tree.h"
#include "data_query.h"
#include "json_parser.h"
#include "string_tree.h"
#include "tree_visualization.h"
#include "tree_visualizer.h"
#include <SFML/Graphics/Font.h>
#include <stdlib.h>

/*TODO: template */
void func1() {
	sfFont* font = sfFont_createFromFile("font.ttf");
	if (!font) {
		return;
	}

	FILE* fp = fopen("data/example.json", "r");
	if (!fp) return;
	char* str = get_file_contents(fp);
	fclose(fp);
	if (!str) {
		return;
	}
	
	StringTreeNode parsed_tree = read_from_json(str);

	free(str);

	const size_t instr_size = 10;

	DataQueryKey* instr[instr_size];
	size_t instr_sizes[instr_size];

	size_t nr_columns = 4;

	DecisionTable decision_table = create_decision_tree_from_parsed_tree(parsed_tree,
		instr, instr_sizes, nr_columns);
	
	free_string_tree_children(parsed_tree);

	int attributes[10] = {0};

	TextTreeNode decision_tree = build_decision_tree(decision_table,
		attributes, 0, 3, 1, NULL, NULL);

	TextTreeVisualization viz =
		tree_visualization_create(&decision_tree, font);

	visualize_tree(&viz);
}

int main() {

	/*func1();*/

	return EXIT_SUCCESS;
}