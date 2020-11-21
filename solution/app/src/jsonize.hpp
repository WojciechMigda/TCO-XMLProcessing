#pragma once

#ifndef APP_SRC_JSONIZE_HPP_
#define APP_SRC_JSONIZE_HPP_

#include "cli_params.hpp"
#include "parser.hpp"

#include <vector>
#include <string>

typedef struct
{
    std::string name;
    std::string contents;
} tagname_w_contents_t;


/*
 * `jsonize` takes tags' data produces by the parser and transforms it into
 * either error message (as string) or vector
 * of pairs [tag name, file contents].
 * Passed `cli_params` control indentation of produced JSON contents.
 */
[[nodiscard]]
neither::Either<std::string, std::vector<tagname_w_contents_t>>
jsonize(tags_tree_t && tags, cli_params_t const & cli_params);


#endif /* APP_SRC_JSONIZE_HPP_ */
