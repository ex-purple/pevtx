#include "evtx.hpp"

namespace pevtx
{

const evtx::chunk_list& evtx::get_chunks() const
{
    return chunks;
};

} // namespace pevtx
