#pragma once

#ifndef APP_SRC_JSONIZE_HPP_
#define APP_SRC_JSONIZE_HPP_

#include "parser.hpp"

#include <vector>
#include <string>

typedef struct
{
    std::string fname;
    std::string contents;
} filename_w_contents_t;


[[nodiscard]]
neither::Either<std::string, std::vector<filename_w_contents_t>> jsonize(tags_tree_t && tags);


#endif /* APP_SRC_JSONIZE_HPP_ */
