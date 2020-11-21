#include "jsonize.hpp"
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


Either<std::string, int>
save_to_files(
    std::vector<tagname_w_contents_t> && to_save,
    std::string const & odir)
{
    using rv_type = Either<std::string, int>;

    for (auto const & fwc : to_save)
    {
        fs::path path(odir);
        path /= fwc.name + ".json";

        std::ofstream ofile(path.string());

        if (ofile.is_open())
        {
            ofile << fwc.contents;
        }
        else
        {
            return rv_type::leftOf(fmt::format("Cannot write to file {}", path.string()));
        }
    }

    return rv_type::rightOf(0);
}
