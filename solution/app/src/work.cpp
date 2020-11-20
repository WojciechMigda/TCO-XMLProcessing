#include "filesystem.hpp"
#include "error_printer.hpp"
#include "parser.hpp"

#include "spdlog/spdlog.h"

#include <string>


int work(std::ifstream & ifile, std::string const & odir)
{
    parse_xml_file(ifile)
        .leftMap(error_printer);

    return 0;
}


int maybe_work(std::string const & ifname, std::string const & odir)
{
    int rv = open_file_r(ifname)
        .leftMap(error_printer)
        .rightMap(
            [&odir](std::ifstream && ifile)
            {
                int rv = check_directory_w(odir)
                    .leftMap(error_printer)
                    .rightMap(
                        [&ifile](std::string const & odir)
                        {
                            work(ifile, odir);

                            return 0;
                        }
                    ).join();

                return rv;
            }
        ).join();

    return rv;
}
