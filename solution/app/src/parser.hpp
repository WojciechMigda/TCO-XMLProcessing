#pragma once

#ifndef APP_SRC_PARSER_HPP_
#define APP_SRC_PARSER_HPP_

#include "neither/either.hpp"

#include <fstream>
#include <map>
#include <set>


using attribute_counts_t = std::map<std::string, unsigned int>;

typedef struct
{
    unsigned int count;
    attribute_counts_t attribute_counts;
    std::set<std::string> children;
} tag_record_t;

using tags_tree_t = std::map<std::string, tag_record_t>;


[[nodiscard]]
neither::Either<std::string, tags_tree_t> parse_xml_file(std::ifstream & ifile);


#endif /* APP_SRC_PARSER_HPP_ */
