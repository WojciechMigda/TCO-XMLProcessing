#include "cli_params.hpp"
#include "jsonize.hpp"
#include "parser.hpp"

#include "json/json.hpp"
#include "neither/either.hpp"

#include <vector>
#include <iterator>

using namespace neither;


/*
 * Example contents:
 * {"attributes": [{"name": "a", "required": true}], "children": [{"name": "C", "required": true}]}
 *
 */
filename_w_contents_t
xform_record(
    std::pair<std::string, tag_record_t> const & kv,
    int const json_indent)
{
    nlohmann::json j;

    j["attributes"] = nlohmann::json::array_t();
    j["children"] = nlohmann::json::array_t();

    auto const & tag_record = kv.second;
    auto const tag_count = tag_record.count;
    auto const & children_counts = tag_record.children_counts;
    auto const & attribute_counts = tag_record.attribute_counts;

    auto count_to_json = [tag_count](auto const & kv) -> nlohmann::json
    {
        nlohmann::json rv;

        rv["name"] = kv.first;
        rv["required"] = (kv.second == tag_count);

        return rv;
    };

    std::transform(children_counts.cbegin(), children_counts.cend(), std::back_inserter(j["children"]), count_to_json);

    std::transform(attribute_counts.cbegin(), attribute_counts.cend(), std::back_inserter(j["attributes"]), count_to_json);

    filename_w_contents_t rv = {kv.first, j.dump(json_indent)};

    return rv;
}


Either<std::string, std::vector<filename_w_contents_t>>
jsonize(tags_tree_t && tags, cli_params_t const & cli_params)
{
    using rv_type = Either<std::string, std::vector<filename_w_contents_t>>;

    std::vector<filename_w_contents_t> rv;
    auto const json_indent = params::json_indent(cli_params);

    try
    {
        std::transform(tags.cbegin(), tags.cend(), std::back_inserter(rv),
            [json_indent](auto const & kv)
            {
                return xform_record(kv, json_indent);
            }
        );
    }
    catch(nlohmann::json::exception & ex)
    {
        return rv_type::leftOf(ex.what());
    }

    return rv_type::rightOf(rv);
}
