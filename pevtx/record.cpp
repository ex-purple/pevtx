#include <pevtx/record.hpp>

namespace pevtx
{

uint64_t record::get_id() const
{
    return id;
}

const binxml_node& record::get_root() const
{
    return root;
}

} // namespace pevtx