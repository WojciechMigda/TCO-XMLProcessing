#include "parser.hpp"

#include "neither/either.hpp"
#include "fmt/format.h"

#include <boost/property_tree/xml_parser.hpp>

#include <fstream>
#include <string>
#include <queue>


using namespace neither;
using boost::property_tree::ptree;


tags_tree_t collect_tag_stats(ptree && pt)
{
    tags_tree_t tags;

    std::queue<std::pair<std::string, ptree>> nodes;
    nodes.emplace("<root>", std::move(pt));

    // BFS
    while (not nodes.empty())
    {
        auto & [name, subtree] = nodes.front();
        fmt::print("{} - children size: {}\n", name, subtree.size());

        for (auto & [child_name, child_subtree] : subtree)
        {
            if (child_name == "<xmlattr>")
            {
                fmt::print("    node {} has {} attrs\n", name, child_subtree.size());
                continue;
            }

            nodes.emplace(child_name, child_subtree);
        }

        nodes.pop();
    }

    return tags;
}


Either<std::string, tags_tree_t> parse_xml_file(std::ifstream & ifile)
{
    using rv_type = Either<std::string, tags_tree_t>;

    ptree pt;

    try
    {
        read_xml(ifile, pt);
    }
    catch(boost::property_tree::xml_parser::xml_parser_error & ex)
    {
        return rv_type::leftOf(fmt::format("Bad XML: {}", ex.what()));
    }

    return rv_type::rightOf(collect_tag_stats(std::move(pt)));
}