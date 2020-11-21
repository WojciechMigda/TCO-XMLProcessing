#pragma once

#ifndef APP_SRC_CLI_PARAMS_HPP_
#define APP_SRC_CLI_PARAMS_HPP_

#include <unordered_map>
#include <variant>
#include <string>

/*
 * These two types are used to build generic placeholder for CLI optional
 * parameters that control some aspect of programs behavior. With that they
 * can be conveniently passed to where they are needed.
 */
using cli_param_value_t = std::variant<int, std::string, bool>;
using cli_params_t = std::unordered_map<std::string, cli_param_value_t>;


/*
 * Convenience companion objects to access parameters stored in `cli_params_t`.
 * They simply extract indicated member from the `variant`.
 */
namespace params
{


/*
 * Retrieve `root_name` value from CLI parameters.
 */
static inline std::string root_name(cli_params_t const & params)
{
    return std::get<std::string>(params.at("root_name"));
}


/*
 * Retrieve `skip_root` value from CLI parameters.
 */
static inline bool skip_root(cli_params_t const & params)
{
    return std::get<bool>(params.at("skip_root"));
}


/*
 * Retrieve `json_indent` value from CLI parameters.
 */
static inline int json_indent(cli_params_t const & params)
{
    return std::get<int>(params.at("json_indent"));
}


} // namespace params


#endif /* APP_SRC_CLI_PARAMS_HPP_ */
