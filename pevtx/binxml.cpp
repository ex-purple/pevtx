#include <boost/property_tree/xml_parser.hpp>
#include <boost/locale.hpp>
#include <pevtx/binxml.hpp>
#include <pevtx/chunk.hpp>

namespace pevtx
{

binxml_parser::binxml_parser()
    : stop(true),
      current_chunk(nullptr),
      root(nullptr)
{

}

void binxml_parser::parse(std::istream &stream, chunk &chunk, binxml_node &node)
{
    set_stream(stream);
    current_chunk = &chunk;
    root = &node;
    stack.clear();

    stop = false;
    do
    {
	auto sym = get();
//std::cout << "token: " << std::hex << std::showbase << sym << std::endl;
	bool more_bits = ((sym & 0xf0) == 0x40);
	switch(static_cast<token>(sym & 0x0f))
	{
	    case token::end_of_stream:
		on_end_of_stream();
		break;
	    
	    case token::open_start_element:
		on_open_start_element(more_bits);
		break;

	    case token::close_start_element:
		on_close_start_element();
		break;

	    case token::close_empty_element:
		on_close_empty_element();
		break;

	    case token::close_element:
		on_close_element();
		break;

	    case token::value:
		on_value();
		break;

	    case token::attribute:
		on_attribute();
		break;

	    case token::cdata_section:
		on_cdata_section();
		break;

	    case token::entity_reference:
		on_entity_reference();
		break;

	    case token::processing_instruction_target:
		on_processing_instruction_target();
		break;

	    case token::processing_instruction_data:
		on_processing_instruction_data();
		break;

	    case token::template_instance:
		on_template_instance();
		break;

	    case token::normal_substitution:
		on_normal_substitution();
		break;

	    case token::conditional_substitution:
		on_conditional_substitution();
		break;

	    case token::start_of_stream:
		on_start_of_stream();
		break;

	    default:
		throw std::runtime_error("Bad stream!");
		break;
	}

    } while(!stop);
}

void binxml_parser::on_end_of_stream()
{
    stack.pop_back();
    stop = true;
}

void binxml_parser::on_open_start_element(bool more_bits)
{
    skip(2); //unknown

    uint32_t size;
    read(size);

    std::string name = read_string();
    if(more_bits) skip(4);

    auto &child = stack.back()->add_child(name, binxml_node());
    stack.push_back(&child);
    stack_path.push_back(name);
}

void binxml_parser::on_close_start_element()
{

}

void binxml_parser::on_close_empty_element()
{
    stack.pop_back();
    stack_path.pop_back();
}

void binxml_parser::on_close_element()
{
    stack.pop_back();
    stack_path.pop_back();
}

void binxml_parser::on_value()
{
    uint8_t type;
    uint16_t size;
    read(type);
    read(size);

    value val;
    val.read(get_stream(), static_cast<value_type>(type), size);

    stack.back()->put_value(val);
    std::string subpath = stack[stack.size() - 2]->back().first;

    if(subpath == "<xmlattr>")
    {
	stack.pop_back();
	stack_path.pop_back();
    }
}

void binxml_parser::on_attribute()
{
    std::string name = read_string();

    std::string path = "<xmlattr>." + name;
    auto &attr = stack.back()->add_child(path, binxml_node());
    stack.push_back(&attr);
    stack_path.push_back(path);
}

void binxml_parser::on_cdata_section()
{

}

void binxml_parser::on_entity_reference()
{

}

void binxml_parser::on_processing_instruction_target()
{

}

void binxml_parser::on_processing_instruction_data()
{

}

void binxml_parser::on_template_instance()
{
    skip(1);

    uint32_t template_id, template_offset, next_offset;
    read(template_id);
    read(template_offset);
    read(next_offset);

    bool new_template = false;
    if(!current_chunk->has_template(template_id))
    {
	new_template = true;

	read(template_id);
	skip(16);

	binxml_template node;
	binxml_parser parser;
	parser.parse(get_stream(), *current_chunk, node);

	current_chunk->add_template(template_id, node);
    }

    auto &tmpl = current_chunk->get_template(template_id);
    *root = tmpl;

    if(new_template)
    {
	uint32_t size;
	read(size);
    }

    std::size_t size = tmpl.subs.size();
    value_spec vs_arr[size];
    for(std::size_t i = 0; i < size; ++i)
    {
	read(vs_arr[i].size);
	read(vs_arr[i].type);
	if(vs_arr[i].type == value_type::wstring_type) vs_arr[i].size /= 2;
    }

    for(std::size_t i = 0; i < size; ++i)
    {
	if(vs_arr[i].type == value_type::bxml_type)
	{
	    binxml_node node;
	    binxml_parser bxml;
	    bxml.parse(get_stream(), *current_chunk, node);

	    auto itr = root->find(tmpl.subs.at(i));
	    if(itr != root->not_found())
	    {
		std::copy(node.begin(), node.end(), std::back_inserter(itr->second));
	    }

	}
	else
	{
	    value val;
	    val.read(get_stream(), vs_arr[i]);
	    root->put(tmpl.subs.at(i), val);
	}
    }


boost::property_tree::xml_writer_settings<char> settings('\t', 1);
boost::property_tree::write_xml(std::cout, *root, settings);
std::cout << std::endl;

}

void binxml_parser::on_normal_substitution()
{
    uint16_t index;
    uint8_t type;

    read(index);
    read(type);

    static_cast<binxml_template*>(root)->subs[index] = get_current_path();
    static_cast<binxml_template*>(root)->subs2[index] = stack.back();

    std::string subpath = stack[stack.size() - 2]->back().first;
    
    if(subpath == "<xmlattr>") 
    {
	stack.pop_back();
	stack_path.pop_back();
    }
}

void binxml_parser::on_conditional_substitution()
{
    uint16_t index;
    uint8_t type;

    read(index);
    read(type);

    static_cast<binxml_template*>(root)->subs[index] = get_current_path();
    static_cast<binxml_template*>(root)->subs2[index] = stack.back();

    std::string subpath = stack[stack.size() - 2]->back().first;
    
    if(subpath == "<xmlattr>") 
    {
	stack.pop_back();
	stack_path.pop_back();
    }
}

void binxml_parser::on_start_of_stream()
{
    skip(3);
    stack.push_back(root);
}

std::string binxml_parser::read_string()
{
    std::string retval;
    uint32_t string_offset;
    read(string_offset);

    if(current_chunk->has_string(string_offset)) retval = current_chunk->get_string(string_offset);
    else
    {
	uint32_t next_offset;
	read(next_offset);

        uint16_t hash, string_length;
        read(hash);
        read(string_length);

        to_utf8(retval, string_length);

	skip(2);

	current_chunk->add_string(string_offset, retval);
    }
    return retval;
}

void binxml_parser::to_utf8(std::string &str, std::size_t length) const
{
    uint16_t buf[length];
    for(std::size_t i = 0; i < length; ++i) read(buf[i]);
    str = boost::locale::conv::utf_to_utf<char>(buf, buf + length);
}

std::string binxml_parser::get_current_path() const
{
    std::string path;
    for(auto &x:stack_path)
    {
	if(!path.empty()) path += '.';
	path += x;
    }
    return path;
}

} // namespace pevtx
 