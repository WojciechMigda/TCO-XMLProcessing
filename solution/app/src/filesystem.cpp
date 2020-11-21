#include "neither/either.hpp"
#include "fmt/format.h"

#include <string>
#include <fstream>

#if defined __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

using namespace neither;


Either<std::string, std::ifstream> open_file_r(std::string const & fname)
{
    using rv_type = Either<std::string, std::ifstream>;

    std::ifstream f(fname);

    if (f.is_open())
    {
        return rv_type::rightOf(std::move(f));
    }
    else
    {
        return rv_type::leftOf(fmt::format("Cannot open file {} for reading", fname));
    }
}


Either<std::string, std::string> check_directory(std::string const & odir)
{
    using rv_type = Either<std::string, std::string>;

    auto const path = fs::path(odir);
    std::error_code ec;

    if (not fs::is_directory(path, ec))
    {
        return rv_type::leftOf(fmt::format("{}: {}", odir, ec.message()));
    }
    else
    {
        return rv_type::rightOf(odir);
    }
}
