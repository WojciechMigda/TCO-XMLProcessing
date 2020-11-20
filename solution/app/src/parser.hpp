#pragma once

#ifndef APP_SRC_PARSER_HPP_
#define APP_SRC_PARSER_HPP_

#include "neither/either.hpp"

#include <fstream>
#include <map>


using attribute_counts_t = std::map<std::string, unsigned int>;
using tag_count_with_attributes_t = std::pair<unsigned int, attribute_counts_t>;
using tags_tree_t = std::map<std::string, tag_count_with_attributes_t>;


[[nodiscard]]
neither::Either<std::string, tags_tree_t> parse_xml_file(std::ifstream & ifile);


#endif /* APP_SRC_PARSER_HPP_ */
