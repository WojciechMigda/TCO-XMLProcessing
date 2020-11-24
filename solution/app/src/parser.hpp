#pragma once

#ifndef APP_SRC_PARSER_HPP_
#define APP_SRC_PARSER_HPP_

#include "cli_params.hpp"
#include "neither/either.hpp"

#include <fstream>
#include <map>
#include <unordered_map>
#include <unordered_set>


/*
 * Employed XML parser produces output which for each tag found in the input
 * has statistics as follows:
 * - tag count, e.g. how many times that tag occurs in the input
 * - attribute counts, that is number of unique occurences of each attribute
 *   that was present in the given tag.
 * - children tags count, that is number of unique occurences of each child
 *   tag.
 *
 * With the above it is possible to distiguish between attributes and child tags
 * whether they are required (always preset) or not.
 * For instance if for a given atttribute or child tag its count equals
 * tag count then such element is required. If it is less then that means that
 * it was not always present, hence, it is not required.
 */

using element_counts_t = std::unordered_map<std::string, unsigned int>;

typedef struct
{
    unsigned int count;
    element_counts_t attribute_counts;
    element_counts_t children_counts;
} tag_record_t;

using tags_tree_t = std::unordered_map<std::string, tag_record_t>;


/*
 * XML parser which takes input file stream, reads from it, and produces
 * either error message (as string) or tags statistics collected from the input.
 */
[[nodiscard]]
neither::Either<std::string, tags_tree_t>
parse_xml_file(
    std::ifstream & ifile,
    std::unordered_set<std::string> const & to_ignore,
    cli_params_t const & cli_params);


#endif /* APP_SRC_PARSER_HPP_ */
