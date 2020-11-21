#pragma once

#ifndef APP_SRC_FILESYSTEM_HPP_
#define APP_SRC_FILESYSTEM_HPP_


#include "neither/either.hpp"

#include <string>
#include <fstream>


/*
 * Check if file identified by name can be open for reading.
 *
 * It returns either error message (as string) or opened input file stream
 * object.
 */
[[nodiscard]]
neither::Either<std::string, std::ifstream> open_file_r(std::string const & fname);


/*
 * Check if passed identifier is a directory.
 *
 * It returns either error message (as string) or the identifier.
 */
[[nodiscard]]
neither::Either<std::string, std::string> check_directory(std::string const & odir);


#endif /* APP_SRC_FILESYSTEM_HPP_ */
