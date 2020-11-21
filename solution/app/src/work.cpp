#include "cli_params.hpp"
#include "filesystem.hpp"
#include "error_printer.hpp"
#include "parser.hpp"
#include "jsonize.hpp"
#include "file_io.hpp"

#include "spdlog/spdlog.h"

#include <string>


/*
 * Process valid input.
 *
 * Read data from the passed input file stream, transform read data, and
 * save results into specified folder.
 *
 * This function and the rest of the code uses Functional Paradigm of error
 * handling using monadic result type
 * (https://en.wikipedia.org/wiki/Result_type)
 * which here is achieved using very smart implementation using `neither`
 * library.
 * Each result type holds either error (the left side), here simply an error
 * string, or successful value (the right side).
 * Subsequent processing usually involves processing of either error or success
 * conditions with right- and left-mapping functions.
 *
 * Full documentation for `neither`: https://github.com/LoopPerfect/neither
 *
 * Applies pipeline consists of the following steps:
 * - parse XML and produce tag statistics,
 * - jsonize tags' statistics into data that can be written to files,
 * - create and write actual files.
 *
 * Error condition (here and in other places) is handled by `error_printer`.
 *
 * I intentionally minimize use of exceptions, as they do not have the property
 * of referential transparency, and they make code less readable and difficult
 * to reason about (see "Out of the Tar Pit" by Moseley and Marks).
 *
 * `join` in the `Either` pipeline 'collapses' left- and right-values into
 * single value, here integer status code.
 */
int work(
    std::ifstream & ifile,
    std::string const & odir,
    cli_params_t const & cli_params)
{
    int rv = parse_xml_file(ifile, cli_params)
        .rightFlatMap(
            [&cli_params](tags_tree_t && tags)
            {
                return jsonize(std::move(tags), cli_params);
            })
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
                int rv = check_directory(odir)
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
