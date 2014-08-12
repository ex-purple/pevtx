#ifndef PEVTX_CHUNK_HPP_INCLUDED
#define PEVTX_CHUNK_HPP_INCLUDED

#include <pevtx/binxml.hpp>
#include <pevtx/record.hpp>

namespace pevtx
{

class chunk
{
public:
    typedef std::map<uint32_t, binxml_template> map_template;

    uint64_t get_first_record_id() const;
    uint64_t get_last_record_id() const;

    bool has_template(uint32_t template_id) const;
    const binxml_template& get_template(uint32_t template_id) const;
    void add_template(uint32_t template_id, const binxml_template &node);

private:
    uint64_t first_record_id, last_record_id;
    map_template templates;

friend class evtx_parser;
};

} // namespace pevtx

#endif //PEVTX_CHUNK_HPP_INCLUDED
