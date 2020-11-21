#pragma once

#ifndef APP_SRC_FILE_IO_HPP_
#define APP_SRC_FILE_IO_HPP_

#include "jsonize.hpp"
#include "neither/either.hpp"

#include <string>


/*
 * `save_to_files` takes vector of pairs [tag name, file contents] and
 * for each entry creates file in output directory `odir` and saves file
 * contents into it.
 *
 * It returns either error message (as string) or integer success code.
 */
[[nodiscard]]
neither::Either<std::string, int>
save_to_files(
    std::vector<tagname_w_contents_t> && to_save,
    std::string const & odir);


#endif /* APP_SRC_FILE_IO_HPP_ */
