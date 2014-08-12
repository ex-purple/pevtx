#include <fstream>
#include <iterator>
#include <algorithm>
#include <pevtx/evtx_parser.hpp>
#include <pevtx/chunk.hpp>

namespace pevtx
{

void evtx_parser::parse(const std::string &filename, evtx &e)
{
    std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
    set_stream(in);
    ev = &e;

    parse_file_header();
    
    while(good())
    {
	char magic[8] = {'E', 'l', 'f', 'C', 'h', 'n', 'k', 0};

	typedef std::istream_iterator<char> iterator;
	std::search(iterator(in), iterator(), std::begin(magic), std::end(magic));
	if(!good()) break;

	chunk ch;
	parse_chunk_header(ch);

	uint64_t id;
	do
	{
	    record rec;
	    parse_record(ch, rec);
	    id = rec.get_id();

	} while(id != ch.get_last_record_id());
    }
}

void evtx_parser::parse_file_header()
{
std::cout << __FUNCTION__ << std::endl;
    std::array<uint8_t, 8> magic;
    uint64_t oldest_chunk;
    uint64_t current_chunk_number;
    uint64_t next_record_number;
    uint32_t header_part_size;
    uint16_t minor_version;
    uint16_t major_version;
    uint16_t header_size;
    uint16_t chunk_count;
    uint32_t flags;
    uint32_t checksum;

    read(magic);
    read(oldest_chunk);
    read(current_chunk_number);
    read(next_record_number);
    read(header_part_size);
    read(minor_version);
    read(major_version);
    read(header_size);
    read(chunk_count);
    skip(76);
    read(flags);
    read(checksum);
}

void evtx_parser::parse_chunk_header(chunk &ch)
{
std::cout << __FUNCTION__ << std::endl;
    uint64_t num_file_rec_first;
    uint64_t num_file_rec_last;
    uint32_t ofs_tables;
    uint32_t ofs_rec_last;
    uint32_t ofs_rec_next;
    uint32_t data_crc;
    uint32_t header_crc;
    std::array<uint32_t, 64> string_table;
    std::array<uint32_t, 32> template_table;

    read(ch.first_record_id);
    read(ch.last_record_id);

    read(num_file_rec_first);
    read(num_file_rec_last);
    read(ofs_tables);
    read(ofs_rec_last);
    read(ofs_rec_next);
    read(data_crc);
    skip(68);
    read(header_crc);
    read(string_table);
    read(template_table);
}

void evtx_parser::parse_record(chunk &ch, record &rec)
{
std::cout << __FUNCTION__ << std::endl;
    std::array<uint8_t, 4> magic;
    uint32_t length1;
    uint64_t filetime;

    read(magic);
    read(length1);
    read(rec.id);
    read(filetime);

    binxml_parser bxp;
    binxml_node node;
    bxp.parse(get_stream(), ch, node);
}

} // namespace pevtx
