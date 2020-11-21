#pragma once

#ifndef APP_SRC_FILE_IO_HPP_
#define APP_SRC_FILE_IO_HPP_

#include "jsonize.hpp"
#include "neither/either.hpp"

#include <string>


[[nodiscard]]
neither::Either<std::string, int> save_to_files(std::vector<filename_w_contents_t> && to_save, std::string const & odir);


#endif /* APP_SRC_FILE_IO_HPP_ */
