#include <pevtx/chunk.hpp>

namespace pevtx
{

uint64_t chunk::get_first_record_id() const
{
    return first_record_id;
}

uint64_t chunk::get_last_record_id() const
{
    return last_record_id;
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