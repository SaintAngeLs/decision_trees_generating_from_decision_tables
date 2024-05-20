#include "build_decision_tree.h"
#include "data_format_conversion.h"
#include "data_query.h"
#include "json_parser.h"
#include "parser_utils.h"
#include "string_tree.h"
#include "text_tree.h"
#include "tree_visualization.h"
#include "tree_visualizer.h"
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Types.h>
#include <SFML/Window/Window.h>
#include <SFML/Window/WindowBase.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xalloc.h"

void func1() {
	/*sfFont* font = sfFont_createFromFile("font.otf");
	if (!font) {
		return;
	}*/

	char* raw_contents = get_str_from_file("data/example.json");
	if (!raw_contents) {
		/*sfFont_destroy(font);*/
		return;
	}

	StringTreeNode parsed_tree = read_from_json(raw_contents);

	free(raw_contents);
	
	/* is nulled */
	if (memcmp(&parsed_tree, (unsigned short[sizeof(StringTreeNode)]){0}, sizeof(StringTreeNode)) == 0) {
		/*sfFont_destroy(font);*/
		return;
	}

	printStringTree(&parsed_tree, 0);

	/*age 10*/
	DataQueryKey i0_0[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	/*states*/
	DataQueryKey i0_1[] = {
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
		(DataQueryKey){DQK_distinct, DQKeyword},
		(DataQueryKey){DQK_toStr, DQKeyword} /*TODO: remove*/
	};

	/*NR children*/
	DataQueryKey i0_2[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){6, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	/*salary*/
	DataQueryKey i0_3[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){7, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	/*marital status*/
	DataQueryKey i1_0[] = {
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

	/*NR children 10*/
	DataQueryKey* i1_1 = i0_2;

	/*salary 10*/
	DataQueryKey* i1_2 = i0_3;

	/*JOIN states 16*/
	DataQueryKey* i2_0 = i0_1;

	/*marital status 10*/
	DataQueryKey* i2_1 = i1_0;



	/*age 10*/
	DataQueryKey* i3_0 = i0_0;

	/*JOIN states 16*/
	DataQueryKey* i3_1 = i0_1;

	/*NR children 10*/
	DataQueryKey* i3_2 = i0_2;

	/*marital status 10*/
	DataQueryKey* i3_3 = i1_0;

	/*NR locations > 1    17*/
	DataQueryKey i3_4[] = {
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
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_changeLvl, DQKeyword},
		(DataQueryKey){DQK_count, DQKeyword},
		(DataQueryKey){DQK_toStr, DQKeyword} /*TODO: remove*/
	};

	/*JOIN Zip codes    16*/
	DataQueryKey i3_5[] = {
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
		(DataQueryKey){2, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_changeLvl, DQKeyword},
		(DataQueryKey){DQK_toStr, DQKeyword} /*TODO: remove*/
	};

	/*salary 10*/
	DataQueryKey* i3_6 = i0_3;

	DataQueryKey** instr_cases[4];

	DataQueryKey* instr_attr0[4];
	DataQueryKey* instr_attr1[3];
	DataQueryKey* instr_attr2[2];
	DataQueryKey* instr_attr3[7];

	instr_cases[0] = instr_attr0;
	instr_cases[1] = instr_attr1;
	instr_cases[2] = instr_attr2;
	instr_cases[3] = instr_attr3;

	instr_attr0[0] = i0_0;
	instr_attr0[1] = i0_1;
	instr_attr0[2] = i0_2;
	instr_attr0[3] = i0_3;

	instr_attr1[0] = i1_0;
	instr_attr1[1] = i1_1;
	instr_attr1[2] = i1_2;

	instr_attr2[0] = i2_0;
	instr_attr2[1] = i2_1;

	instr_attr3[0] = i3_0;
	instr_attr3[1] = i3_1;
	instr_attr3[2] = i3_2;
	instr_attr3[3] = i3_3;
	instr_attr3[4] = i3_4;
	instr_attr3[5] = i3_5;
	instr_attr3[6] = i3_6;

	size_t* instr_sizes_cases[4];

	size_t instr_size0[] = {10, 17, 10, 10};
	size_t instr_size1[] = {10, 10, 10};
	size_t instr_size2[] = {17, 10};
	size_t instr_size3[] = {10, 17, 10, 10, 17, 16, 10};

	instr_sizes_cases[0] = instr_size0;
	instr_sizes_cases[1] = instr_size1;
	instr_sizes_cases[2] = instr_size2;
	instr_sizes_cases[3] = instr_size3;

	size_t nr_columns[4] = {4, 3, 2, 7};

	char** titles[4];

	char* titles_0[] = { "Age", "States", "Children", "Salary" };
	char* titles_1[] = { "Marital status", "Children", "Salary" };
	char* titles_2[] = { "States", "Marital status" };
	char* titles_3[] = { "Age", "States", "Children", "Marital status", "Multi_location", "Zip codes", "Salary" };

	titles[0] = titles_0;
	titles[1] = titles_1;
	titles[2] = titles_2;
	titles[3] = titles_3;

	int* attr[4];
	int attr0[] = {0, 0, 0, 1};
	int attr1[] = {0, 0, 1};
	int attr2[] = {0, 1};
	int attr3[] = {0, 0, 0, 0, 0, 0, 1};

	int* numAttr[4];
	int numAttr0[] = {1, 0, 1, 1};
	int numAttr1[] = {0, 0, 1};
	int numAttr2[] = {0, 0};
	int numAttr3[] = {1, 0, 0, 0, 0, 0, 1};

	attr[0] = attr0;
	attr[1] = attr1;
	attr[2] = attr2;
	attr[3] = attr3;

	numAttr[0] = numAttr0;
	numAttr[1] = numAttr1;
	numAttr[2] = numAttr2;
	numAttr[3] = numAttr3;

	int tgt_attr[] = {3, 2, 1, 6};

	int depth[4] = {1,2,1,1};

	DecisionTable decision_tables[4];

	decision_tables[0] = create_decision_table_from_parsed_tree(parsed_tree, titles[0], instr_cases[0],
			instr_sizes_cases[0], nr_columns[0]);
	decision_tables[1] = create_decision_table_from_parsed_tree(parsed_tree, titles[1], instr_cases[1],
			instr_sizes_cases[1], nr_columns[1]);
	decision_tables[2] = create_decision_table_from_parsed_tree(parsed_tree, titles[2], instr_cases[2],
			instr_sizes_cases[2], nr_columns[2]);
	decision_tables[3] = create_decision_table_from_parsed_tree(parsed_tree, titles[3], instr_cases[3],
			instr_sizes_cases[3], nr_columns[3]);

	free_string_tree_children(parsed_tree);

	for (int i = 0; i < 4; ++i) {
		if (!decision_tables[i].titles) {
			/*sfFont_destroy(font);*/
			return;
		}

		print_decision_table(decision_tables[i]);

		TextTreeNode decision_tree = build_decision_tree(decision_tables[i],
			attr[i], numAttr[i], 1, depth[i], tgt_attr[i], NULL, NULL);
		
		if (!decision_tree.node_text) {
			fprintf(stderr, "No decision tree %d\n", i);
			free_decision_table(decision_tables[i], 0);
			/*sfFont_destroy(font);*/
			return;
		}

		print_decision_tree(decision_tree, 0);

		free_text_tree(decision_tree);
		free_decision_table(decision_tables[i], 0);
	}

	
	/*sfFont_destroy(font);*/
	return;

	
	
	

	

	

	/*TextTreeVisualization viz =
		tree_visualization_create(&decision_tree, font);

	visualize_tree(&viz);*/

	
}

void func2() {
	sfFont* font = sfFont_createFromFile("font.otf");
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

	/*age 10*/
	DataQueryKey i0_0[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	/*states*/
	DataQueryKey i0_1[] = {
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
		(DataQueryKey){DQK_distinct, DQKeyword},
		(DataQueryKey){DQK_toStr, DQKeyword} /*TODO: remove*/
	};

	/*NR children*/
	DataQueryKey i0_2[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){6, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	/*salary*/
	DataQueryKey i0_3[] = {
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){0, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){1, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_foreach, DQKeyword},
		(DataQueryKey){7, DQInt},
		(DataQueryKey){DQK_propId, DQKeyword},
		(DataQueryKey){DQK_fetch, DQKeyword}
	};

	/*marital status*/
	DataQueryKey i1_0[] = {
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

	/*NR children 10*/
	DataQueryKey* i1_1 = i0_2;

	/*salary 10*/
	DataQueryKey* i1_2 = i0_3;

	DataQueryKey* instr_attr1[3];
	instr_attr1[0] = i1_0;
	instr_attr1[1] = i1_1;
	instr_attr1[2] = i1_2;

	char* titles_1[] = { "Marital status", "Children", "Salary" };

	size_t instr_size1[] = {10, 10, 10};

	DecisionTable decision_table = create_decision_table_from_parsed_tree(parsed_tree,
	 titles_1, instr_attr1,
			instr_size1, 3);

	free_string_tree_children(parsed_tree);

		if (!decision_table.titles) {
			sfFont_destroy(font);
			return;
		}

		print_decision_table(decision_table);

		int attr1[] = {0, 0, 1};
		int numAttr1[] = {0, 0, 1};

		TextTreeNode decision_tree = build_decision_tree(decision_table,
			attr1, numAttr1, 1, 2, 2, NULL, NULL);
		
		if (!decision_tree.node_text) {
			fprintf(stderr, "No decision tree\n");
			free_decision_table(decision_table, 0);
			sfFont_destroy(font);
			return;
		}

		print_decision_tree(decision_tree, 0);

		enable_forking_allocation = 0;

		TextTreeVisualization viz = tree_visualization_create(&decision_tree, font);
		if (!memcmp(&viz, &(TextTreeVisualization){0}, sizeof(TextTreeVisualization))) {
			fprintf(stderr, "Failed to create tree visualization\n");
			free_text_tree(decision_tree);
			free_decision_table(decision_table, 0);
			sfFont_destroy(font);
			return;
		}

		visualize_tree(&viz);

	free_visualization_tree(&viz);
		free_text_tree(decision_tree);
		free_decision_table(decision_table, 0);	
	sfFont_destroy(font);
	return;
}

void func3() {

}

int main(int argc, char** args) {
	if (argc > 1) {
		if (args[1][0] == '1') {
			func1();
		} else if (args[1][0] == '2') {
			func2();
		} else if (args[1][0] == '3') {
			func3();
		}
	}
	else {
		fprintf(stderr, "Usage: n\nn - nr of test function\n");
	}
	return EXIT_SUCCESS;
}