#include "work.hpp"

#include "spdlog/spdlog.h"
#include "clipp.hpp"

#include <iostream>


int main(int argc, char **argv)
{
    std::string xml_ifname;
    std::string out_dir;

    auto cli = (
        clipp::required("--input", "-i").doc("Input XML file") & clipp::value("XML data file to read from", xml_ifname),
        clipp::required("--output-dir", "-o").doc("Destination folder where output files are to be created") & clipp::value("Destination folder", out_dir)
    );


    if (not clipp::parse(argc, argv, cli))
    {
        std::cout << clipp::make_man_page(cli, argv[0]);

        return -1;
    }
    else
    {
        spdlog::info("Start");

        return maybe_work(xml_ifname, out_dir);
    }
}
