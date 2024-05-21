#ifndef MINIML_XML_PARSER_H
#define MINIML_XML_PARSER_H
#include "string_tree.h"

/** \brief Parse an XML document.
 *         Create the named string tree from the XML contents.
 *
 * This function parses XML documents with only essential contents such as
 * properties and nested objects. Properties are treated as children of the
 * object.
 *
 * @param xml_contents A pointer to the string that contains XML document
 * @returns A string tree with children if succeeded, otherwise a tree with no
 * contents
 */
StringTreeNode read_from_xml(const char *xml_contents);

#endif /* MINIML_XML_PARSER_H */