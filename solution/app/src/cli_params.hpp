#pragma once

#ifndef APP_SRC_CLI_PARAMS_HPP_
#define APP_SRC_CLI_PARAMS_HPP_

#include <unordered_map>
#include <variant>
#include <string>


using cli_param_value_t = std::variant<int, std::string, bool>;

using cli_params_t = std::unordered_map<std::string, cli_param_value_t>;


namespace params
{

static inline std::string root_name(cli_params_t const & params)
{
    return std::get<std::string>(params.at("root_name"));
}


static inline bool skip_root(cli_params_t const & params)
{
    return std::get<bool>(params.at("skip_root"));
}


static inline int json_indent(cli_params_t const & params)
{
    return std::get<int>(params.at("json_indent"));
}


} // namespace params


#endif /* APP_SRC_CLI_PARAMS_HPP_ */
