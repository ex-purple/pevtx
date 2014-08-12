#ifndef PEVTX_READER_HPP_INCLUDED
#define PEVTX_READER_HPP_INCLUDED

#include <istream>
#include <array>

namespace pevtx
{

class reader
{
public:
    bool good() const;
    std::size_t position() const;
    std::istream& get_stream();
    void set_stream(std::istream &stream);    

    void skip(std::size_t size) const;

    int get() const;

    template<typename T> 
    void read(T &data) const;

    template<typename T>
    void read(T *data, std::size_t size) const;

    template<typename T, std::size_t Size>
    void read(std::array<T, Size> &data) const;

    void read(std::wstring &data, std::size_t size) const;

protected:
    reader();
    reader(std::istream &stream);

private:
    void check() const; 

    std::istream *in;
};

template<typename T> 
void reader::read(T &data) const
{
    check();
    in->read(reinterpret_cast<char*>(&data), sizeof(data));
}

template<typename T>
void reader::read(T *data, std::size_t size) const
{
    check();
    in->read(reinterpret_cast<char*>(data), size);
}

template<typename T, std::size_t Size>
void reader::read(std::array<T, Size> &data) const
{
    check();
    in->read(reinterpret_cast<char*>(data.data()), sizeof(data));
}

} // namespace pevtx

#endif //READER_HPP_INCLUDED
