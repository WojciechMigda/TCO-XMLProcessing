#include "cli_params.hpp"
#include "parser.hpp"

#include "neither/either.hpp"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

#include <boost/property_tree/xml_parser.hpp>

#include <fstream>
#include <string>
#include <queue>
#include <unordered_set>


using namespace neither;
using boost::property_tree::ptree;


/*
 * Take boost's property tree produced by boost's XML parser and traverse it
 * to produce required tag statistics.
 *
 * It simply does BFS of the tree using node queue and for each tag retrieves
 * attributes, which are stored in `ptree` as a special node called '<xmlattr>'.
 *
 * It makes sure that children tags and attributes are counted as unique
 * occurences. For instance with duplicated attribute like here:
 *
 * <tag attr="1" attr="2"/>
 *
 * this code will increase attr count by 1. Without this with the input
 * as follows:
 *
 * <tag attr="1" attr="2"/><tag/>
 *
 * would have total count of 'attr' for tag 'tag' equal 2. This would later
 * cause 'attr' being marked as 'required' if 'tag' occured only these two times.
 *
 * Passed `cli_params` allow to control handling of the root node. `ptree`
 * produced by boost's XML parser has an unnamed root node. For the convenience
 * of this code it is given custom name. The user can decide whether this root
 * node will or will not be included in final result, and what the custom name
 * shall be. By default this root node is skipped.
 */
tags_tree_t collect_tag_stats(ptree && pt, cli_params_t const & cli_params)
{
    tags_tree_t tags;
    std::queue<std::pair<std::string, ptree>> nodes;

    nodes.emplace(params::root_name(cli_params), std::move(pt));

    // BFS
    while (not nodes.empty())
    {
        std::unordered_set<std::string> unique_children;

        auto & [name, subtree] = nodes.front();

        auto & tag_record = tags[name];

        // bump counter for the current tag
        ++tag_record.count;

        spdlog::debug("{} [{}] - children size: {}", name, tag_record.count, subtree.size());

        for (auto & [child_name, child_subtree] : subtree)
        {
            if (child_name == "<xmlattr>")
            {
                std::unordered_set<std::string> unique_atts;

                // update known attributes and their counts

                for (auto const & kv : child_subtree)
                {
                    unique_atts.insert(kv.first);
                }
                for (auto const & att_name : unique_atts)
                {
                    tag_record.attribute_counts[att_name] += 1;
                }

                spdlog::debug("    node {} has {} attrs", name, child_subtree.size());
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

    if (params::skip_root(cli_params))
    {
        tags.erase(params::root_name(cli_params));
    }

    return tags;
}


Either<std::string, tags_tree_t>
parse_xml_file(std::ifstream & ifile, cli_params_t const & cli_params)
{
    using rv_type = Either<std::string, tags_tree_t>;

    ptree pt;

    try
    {
        // call boost's XML parser
        read_xml(ifile, pt);
    }
    catch(boost::property_tree::xml_parser::xml_parser_error & ex)
    {
        return rv_type::leftOf(fmt::format("Bad XML: {}", ex.what()));
    }

    return rv_type::rightOf(collect_tag_stats(std::move(pt), cli_params));
}
