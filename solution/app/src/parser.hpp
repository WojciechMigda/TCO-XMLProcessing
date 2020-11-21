#pragma once

#ifndef APP_SRC_PARSER_HPP_
#define APP_SRC_PARSER_HPP_

#include "cli_params.hpp"
#include "neither/either.hpp"

#include <fstream>
#include <map>
#include <set>


using element_counts_t = std::map<std::string, unsigned int>;

typedef struct
{
    unsigned int count;
    element_counts_t attribute_counts;
    element_counts_t children_counts;
} tag_record_t;

using tags_tree_t = std::map<std::string, tag_record_t>;


[[nodiscard]]
neither::Either<std::string, tags_tree_t>
parse_xml_file(std::ifstream & ifile, cli_params_t const & cli_params);


#endif /* APP_SRC_PARSER_HPP_ */
