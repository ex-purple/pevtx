#ifndef PEVTX_EVTX_HPP_INCLUDED
#define PEVTX_EVTX_HPP_INCLUDED

#include <pevtx/chunk.hpp>

namespace pevtx
{

class evtx
{
public:
    using chunk_list = std::vector<chunk>;

    const chunk_list& get_chunks() const;

private:
    chunk_list chunks;

friend class evtx_parser;
};

} // namespace evtx

#endif //PEVTX_EVTX_HPP_INCLUDED
