#pragma once

#ifndef APP_SRC_FILESYSTEM_HPP_
#define APP_SRC_FILESYSTEM_HPP_


#include "neither/either.hpp"

#include <string>
#include <fstream>

[[nodiscard]]
neither::Either<std::string, std::ifstream> open_file_r(std::string const & fname);

[[nodiscard]]
neither::Either<std::string, std::string> check_directory_w(std::string const & odir);


#endif /* APP_SRC_FILESYSTEM_HPP_ */
