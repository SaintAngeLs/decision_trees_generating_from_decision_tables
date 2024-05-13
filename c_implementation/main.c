#include "build_decision_tree.h"
#include "data_format_conversion.h"
#include "data_query.h"
#include "json_parser.h"
#include "parser_utils.h"
#include "string_tree.h"
#include "tree_visualization.h"
#include "tree_visualizer.h"
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/Types.h>
#include <stdlib.h>
#include <string.h>

void func1() {
	sfFont* font = sfFont_createFromFile("font.ttf");
	if (!font) {
		return;
	}

	char* raw_contents = get_str_from_file("data/example.json");
	if (!raw_contents) {
		sfFont_destroy(font);
		return;
	}

	StringTreeNode parsed_tree = read_from_json(raw_contents);

	free(raw_contents);
	
	/* is nulled */
	if (memcmp(&parsed_tree, (unsigned short[sizeof(StringTreeNode)]){0}, sizeof(StringTreeNode)) == 0) {
		sfFont_destroy(font);
		return;
	}

	printStringTree(&parsed_tree, 0);

	DataQueryKey* instr[2];
	DataQueryKey i1[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){3, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	DataQueryKey i2[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){5, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){3, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_changeLvl, DQKeyword},
		(DataQueryKey){DQK_toStr, DQKeyword}
	};

	instr[0] = i1;
	instr[1] = i2;

	size_t instr_sizes[2] = {10, 16};
	size_t nr_columns = 2;

	char* titles[2];
	titles[0] = "Marital status";
	titles[1] = "Locations";

	DecisionTable decision_table =
		create_decision_table_from_parsed_tree(parsed_tree, titles, instr,
			instr_sizes, nr_columns);

	free_string_tree_children(parsed_tree);

	if (!decision_table.titles) {
		sfFont_destroy(font);
		return;
	}

	print_decision_table(decision_table);

	/* TODO */
	free_decision_table(decision_table, 0);

	sfFont_destroy(font);
	return;

	
	int attributes[2] = {0};

	TextTreeNode decision_tree = build_decision_tree(decision_table,
		attributes, 0, 1, 1, NULL, NULL);
	
	


	
	
	

	

	

	TextTreeVisualization viz =
		tree_visualization_create(&decision_tree, font);

	visualize_tree(&viz);

	
}

void func2() {

}

void func3() {

}

int main() {

	func1();
	func2();
	func3();

	return EXIT_SUCCESS;
}