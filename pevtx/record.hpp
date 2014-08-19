#ifndef PEVTX_RECORD_HPP_INCLUDED
#define PEVTX_RECORD_HPP_INCLUDED

#include <pevtx/binxml.hpp>

namespace pevtx
{

class record
{
public:
    uint64_t get_id() const;
    const binxml_node& get_root() const;

private:
    uint64_t id;
    binxml_node root;

friend class evtx_parser;
};

}

#endif //PEVTX_RECORD_HPP_INCLUDED