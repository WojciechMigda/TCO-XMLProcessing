#pragma once

#ifndef APP_SRC_WORK_HPP_
#define APP_SRC_WORK_HPP_

#include "cli_params.hpp"

#include <string>


int maybe_work(
    std::string const & ifname,
    std::string const & odir,
    cli_params_t const & cli_params);


#endif /* APP_SRC_WORK_HPP_ */
