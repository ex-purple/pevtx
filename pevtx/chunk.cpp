#include <pevtx/chunk.hpp>

namespace pevtx
{

uint32_t chunk::get_offset() const
{
    return offset;
}

uint64_t chunk::get_first_record_id() const
{
    return first_record_id;
}

uint64_t chunk::get_last_record_id() const
{
    return last_record_id;
}

const chunk::records_list& chunk::get_records() const
{
    return records;
}

bool chunk::has_string(uint32_t string_id) const
{
    return (strings.find(string_id) != strings.end());
}

const std::string& chunk::get_string(uint32_t string_id) const
{
    return strings.at(string_id);
}

void chunk::add_string(uint32_t string_id, const std::string &str)
{
    strings[string_id] = str;
}

bool chunk::has_template(uint32_t template_id) const
{
    return (templates.find(template_id) != templates.end());
}

const binxml_template& chunk::get_template(uint32_t template_id) const
{
    return templates.at(template_id);
}

void chunk::add_template(uint32_t template_id, const binxml_template &node)
{
    templates[template_id] = node;
}

}