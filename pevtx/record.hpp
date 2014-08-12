#ifndef PEVTX_RECORD_HPP_INCLUDED
#define PEVTX_RECORD_HPP_INCLUDED

#include <cstdint>

namespace pevtx
{

class record
{
public:
    uint64_t get_id() const;

private:
    uint64_t id;

friend class evtx_parser;
};

}

#endif //PEVTX_RECORD_HPP_INCLUDED