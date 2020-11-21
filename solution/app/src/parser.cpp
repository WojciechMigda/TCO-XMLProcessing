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
        std::set<std::string> unique_children;

        auto & [name, subtree] = nodes.front();

        auto & tag_record = tags[name];

        // bump counter for the current tag
        ++tag_record.count;

        fmt::print("{} [{}] - children size: {}\n", name, tag_record.count, subtree.size());

        for (auto & [child_name, child_subtree] : subtree)
        {
            if (child_name == "<xmlattr>")
            {
                std::set<std::string> unique_atts;

                // update known attributes and their counts

                for (auto const & kv : child_subtree)
                {
                    unique_atts.insert(kv.first);
                }
                for (auto const & att_name : unique_atts)
                {
                    tag_record.attribute_counts[att_name] += 1;
                }

                fmt::print("    node {} has {} attrs\n", name, child_subtree.size());
                continue;
            }

            // update known children and their counts
            unique_children.insert(child_name);

            nodes.emplace(child_name, child_subtree);
        }
        for (auto const & child_name : unique_children)
        {
            tag_record.children_counts[child_name] += 1;
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
