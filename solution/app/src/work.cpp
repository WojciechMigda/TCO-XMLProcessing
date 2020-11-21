#include "cli_params.hpp"
#include "filesystem.hpp"
#include "error_printer.hpp"
#include "parser.hpp"
#include "jsonize.hpp"
#include "file_io.hpp"

#include "spdlog/spdlog.h"

#include <string>


int work(
    std::ifstream & ifile,
    std::string const & odir,
    cli_params_t const & cli_params)
{
    int rv = parse_xml_file(ifile, cli_params)
        .rightFlatMap(jsonize)
        .rightFlatMap(
            [&odir, &cli_params](auto && fnames_with_contents)
            {
                return save_to_files(std::move(fnames_with_contents), odir);
            }
        )
        .leftMap(error_printer)
        .join()
        ;

    return rv;
}


int maybe_work(
    std::string const & ifname,
    std::string const & odir,
    cli_params_t const & cli_params)
{
    int rv = open_file_r(ifname)
        .leftMap(error_printer)
        .rightMap(
            [&odir, &cli_params](std::ifstream && ifile)
            {
                int rv = check_directory_w(odir)
                    .leftMap(error_printer)
                    .rightMap(
                        [&ifile, &cli_params](std::string const & odir)
                        {
                            work(ifile, odir, cli_params);

                            return 0;
                        }
                    ).join();

                return rv;
            }
        ).join();

    return rv;
}
