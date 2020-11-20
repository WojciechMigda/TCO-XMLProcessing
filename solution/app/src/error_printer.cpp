#include "spdlog/spdlog.h"

#include <string>

int error_printer(std::string && error_message)
{
    spdlog::error(error_message);

    return -1;
}
