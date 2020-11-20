#include "parser.hpp"

#include "neither/either.hpp"
#include "fmt/format.h"

#include <boost/property_tree/xml_parser.hpp>

#include <fstream>
#include <string>

using namespace neither;


Either<std::string, tags_tree_t> parse_xml_file(std::ifstream & ifile)
{
    using rv_type = Either<std::string, tags_tree_t>;

    using boost::property_tree::ptree;

    ptree pt;

    try
    {
        read_xml(ifile, pt);
    }
    catch(boost::property_tree::xml_parser::xml_parser_error & ex)
    {
        return rv_type::leftOf(fmt::format("Bad XML: {}", ex.what()));
    }

    tags_tree_t tags;

    ;

    return rv_type::rightOf(std::move(tags));
}