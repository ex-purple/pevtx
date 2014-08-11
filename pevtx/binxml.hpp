#ifndef PPEVTX_BINXML_HPP_INCLUDED
#define PREVTX_BINXML_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <pevtx/reader.hpp>
#include <pevtx/value.hpp>

namespace pevtx
{

enum class token
{
    end_of_stream = 0,
    open_start_element = 0x01,
    close_start_element = 0x02,
    close_empty_element = 0x03,
    close_element = 0x04,
    value = 0x05,
    attribute = 0x06,
    cdata_section = 0x07,
    entity_reference = 0x08,
    processing_instruction_target = 0x0a,
    processing_instruction_data = 0x0b,
    template_instance = 0xc,
    normal_substitution = 0x0d,
    conditional_substitution = 0x0e,
    start_of_stream = 0x0f
};


typedef boost::property_tree::basic_ptree<std::string, value> binxml_node;

class binxml_parser : public reader
{

};

}

#endif //PREVTX_BINXML_INCLUDED