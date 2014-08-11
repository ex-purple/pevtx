#include <pevtx/reader.hpp>

namespace pevtx
{

bool reader::good() const
{
    return ((in != nullptr) && (in->good()));
}

void reader::set_stream(std::istream &stream)
{
    in = &stream;
}

void reader::skip(std::size_t size) const
{
    check();
    in->seekg(size, in->cur);
}

void reader::read(std::wstring &data, std::size_t size) const
{
    data.clear();    
    check();

    uint16_t buf[size];
    for(uint16_t i = 0; i < size; ++i) read(buf[i]);
    std::copy(buf, buf + size, std::back_inserter(data));    
}

reader::reader()
    : in(nullptr)
{

}

reader::reader(std::istream &stream)
    : in(&stream)
{

}

void reader::check() const
{
    if(!in || !in->good()) throw std::runtime_error("Bad stream!");
}

} // namespace pevtx
