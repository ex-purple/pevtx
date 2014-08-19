#ifndef PEVTX_CHUNK_HPP_INCLUDED
#define PEVTX_CHUNK_HPP_INCLUDED

#include <pevtx/binxml.hpp>
#include <pevtx/record.hpp>

namespace pevtx
{

class chunk
{
public:
    using records_list = std::vector<record>;
    using map_string = std::map<uint32_t, std::string>;
    using map_template = std::map<uint32_t, binxml_template>;

    uint32_t get_offset() const;
    uint64_t get_first_record_id() const;
    uint64_t get_last_record_id() const;

    bool has_string(uint32_t string_id) const;
    const std::string& get_string(uint32_t string_id) const;
    void add_string(uint32_t string_id, const std::string &str);

    bool has_template(uint32_t template_id) const;
    const binxml_template& get_template(uint32_t template_id) const;
    void add_template(uint32_t template_id, const binxml_template &node);

    const records_list& get_records() const;

private:
    uint32_t offset;
    uint64_t first_record_id, last_record_id;
    map_string strings;
    map_template templates;
    records_list records;

friend class evtx_parser;
};

} // namespace pevtx

#endif //PEVTX_CHUNK_HPP_INCLUDED
