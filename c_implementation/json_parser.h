#ifndef MINIML_JSON_PARSER_H
#define MINIML_JSON_PARSER_H
#include "string_tree.h"

/** \brief Parse a JSON document.
 *         Create the named string tree from the JSON contents.
 *
 * This function parses JSON documents with only essential contents such as properties
 * and arrays. Properties are treated as children of the object.
 *
 * @param json_contents A pointer to the string that contains JSON document
 * @returns A string tree with children if succeeded, otherwise a tree with no contents
 */
StringTreeNode read_from_json(const char* json_contents);

#endif /* MINIML_JSON_PARSER_H */