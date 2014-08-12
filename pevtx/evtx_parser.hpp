#ifndef PEVTX_EVTX_PARSER_HPP
#define PEVTX_EVTX_PARSER_HPP

#include <pevtx/reader.hpp>
#include <pevtx/evtx.hpp>

namespace pevtx
{

class evtx_parser : public reader
{
public:
    void parse(const std::string &filename, evtx &e);

private:
    void parse_file_header();
    void parse_chunk_header(chunk &ch);
    void parse_record(chunk &ch, record &rec);

    evtx *ev;
};

}

#endif //PEVTX_EVTX_PARSER_HPP
