#ifndef PEVTX_BINXML_HPP_INCLUDED
#define PEVTX_BINXML_HPP_INCLUDED

#include <map>
#include <unordered_map>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <pevtx/reader.hpp>
#include <pevtx/value.hpp>

namespace boost { namespace property_tree
{
    template<>
    struct translator_between<pevtx::value, std::string>
    {
	using type = id_translator<std::string>;
    };
}}

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

using binxml_node = boost::property_tree::basic_ptree<std::string, value>;
using path = std::vector<std::size_t>;
using map_substitution = std::unordered_map<uint16_t, path>;

class binxml_template : public binxml_node
{
public:
    virtual ~binxml_template() {}

    static binxml_node* get_child(binxml_node *node, std::size_t pos);
    std::size_t count_substitutions() const;
    binxml_node* get_substitution(uint16_t index, binxml_node *root) const;
    void add_substitution(uint16_t index, const path &p);
    
private:
    map_substitution subs;
};

class chunk;

class binxml_parser : public reader
{
public:
    binxml_parser();

    void parse(std::istream &stream, chunk &chunk, binxml_node &node, bool template_definition = false);

private:
    void on_end_of_stream();
    void on_open_start_element(bool more_bits);
    void on_close_start_element();
    void on_close_empty_element();
    void on_close_element();
    void on_value(bool more_bits);
    void on_attribute(bool more_bits);
    void on_cdata_section();
    void on_entity_reference();
    void on_processing_instruction_target();
    void on_processing_instruction_data();
    void on_template_instance();
    void on_normal_substitution();
    void on_conditional_substitution();
    void on_start_of_stream();
    std::string read_string();
    void to_utf8(std::string &str, std::size_t length) const;
    std::string get_current_path() const;

    bool stop, is_template_definition;
    chunk *current_chunk;
    binxml_node *root;
    std::vector<binxml_node*> stack;
    std::vector<std::size_t> stack_path;
};

}

#endif //PEVTX_BINXML_INCLUDED
