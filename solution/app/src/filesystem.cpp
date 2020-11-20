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
    std::ifstream f(fname);

    if (f.is_open())
    {
        return Either<std::string, std::ifstream>::rightOf(std::move(f));
    }
    else
    {
        return Either<std::string, std::ifstream>::leftOf(fmt::format("Cannot open file {} for reading", fname));
    }
}


Either<std::string, std::string> check_directory_w(std::string const & odir)
{
    auto const path = fs::path(odir);
    std::error_code ec;

    if (not fs::is_directory(path, ec))
    {
        return Either<std::string, std::string>::leftOf(fmt::format("{}: {}", odir, ec.message()));
    }
    else
    {
        return Either<std::string, std::string>::rightOf(odir);
    }
}
