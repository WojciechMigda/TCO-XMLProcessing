#pragma once

#ifndef APP_SRC_WORK_HPP_
#define APP_SRC_WORK_HPP_

#include "cli_params.hpp"

#include <string>


/*
 * Attempt to process input.
 * This function will validate the input, before processing. If the validation
 * is unsuccessful then a relevant error message will be printed. Otherwise
 * processing pipeline will be launched.
 *
 * Returns integer status code to return from the program.
 */
int maybe_work(
    std::string const & ifname,
    std::string const & odir,
    cli_params_t const & cli_params);


#endif /* APP_SRC_WORK_HPP_ */
