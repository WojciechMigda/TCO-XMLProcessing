#include "work.hpp"

#include "spdlog/spdlog.h"
#include "clipp.hpp"

#include <iostream>
#include <string>

static std::string to_string(bool v)
{
    return v ? "true" : "false";
}


static void set_log_level(bool verbose, bool debug)
{
    if (verbose)
    {
        spdlog::set_level(spdlog::level::info);
    }
    else if (debug)
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else
    {
        spdlog::set_level(spdlog::level::warn);
    }
}


int main(int argc, char **argv)
{
    std::string xml_ifname;
    std::string out_dir;
    std::string root_name("<ROOT>");
    bool skip_root = true;
    bool verbose = false;
    bool debug = false;

    auto cli = (
        clipp::required("--input", "-i").doc("Input XML file") & clipp::value("XML data file to read from", xml_ifname),
        clipp::required("--output-dir", "-o").doc("Destination folder where output files are to be created") & clipp::value("Destination folder", out_dir),
        clipp::option("--root-name").doc("Name for the root node to use, if enabled") & clipp::value("Name for the root node=" + root_name, root_name),
        (
            clipp::option("--skip-root").set(skip_root, true) |
            clipp::option("--no-skip-root").set(skip_root, false)
        ).doc("Include or do not include root node in created json files, default=" + to_string(skip_root)),
        clipp::option("--verbose", "-v").set(verbose, true).doc("Enable verbose output, default=" + to_string(verbose)),
        clipp::option("--debug").set(debug, true).doc("Enable debug output, default=" + to_string(debug))
    );

    if (not clipp::parse(argc, argv, cli))
    {
        std::cout << clipp::make_man_page(cli, argv[0]);

        return -1;
    }
    else
    {
        set_log_level(verbose, debug);

        spdlog::info("Start");

        auto rv = maybe_work(xml_ifname, out_dir);

        spdlog::info("Done");

        return rv;
    }
}
