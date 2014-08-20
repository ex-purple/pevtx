#ifndef PEVTX_VALUE_HPP_INCLUDED
#define PEVTX_VALUE_HPP_INCLUDED

#include <array>
#include <memory>
#include <vector>
#include <boost/variant.hpp>

namespace pevtx
{

enum class value_type : uint16_t
{
    null_type = 		0,
    wstring_type = 		0x01,
    string_type = 		0x02,
    int8_type = 		0x03,
    uint8_type = 		0x04,
    int16_type = 		0x05,
    uint16_type = 		0x06,
    int32_type = 		0x07,
    uint32_type = 		0x08,
    int64_type = 		0x09,
    uint64_type = 		0x0a,
    float_type = 		0x0b,
    double_type = 		0x0c,
    boolean_type =		0x0d,
    binary_type =		0x0e,
    guid_type = 		0x0f,
    size_type = 		0x10,
    filetime_type =		0x11,
    systemtime_type = 		0x12,
    sid_type =			0x13,
    hex32_type =		0x14,
    hex64_type =		0x15,
    bxml_type =			0x21,
    wstring_array_type =	0x81
};

struct null
{

};

struct guid
{
    uint32_t			data1;
    uint16_t			data2;
    uint16_t			data3;
    std::array<uint8_t, 8>	data4;
};

template<typename T>
struct hex
{
    hex(T value = 0) : data(value) {}
    T data;
};

using hex32 = hex<uint32_t>;
using hex64 = hex<uint64_t>;

struct sid
{
    uint8_t version;
    uint64_t id;
    std::vector<uint32_t> elements;
};


template<value_type ValType = value_type::null_type>
struct value_helper 
{
    using type = null;
};

template<>
struct value_helper<value_type::wstring_type>
{
    using type = std::wstring;
};

template<>
struct value_helper<value_type::string_type>
{
    using type = std::string;
};

template<>
struct value_helper<value_type::int8_type>
{
    using type = int8_t;
};

template<>
struct value_helper<value_type::uint8_type>
{
    using type =  uint8_t;
};

template<>
struct value_helper<value_type::int16_type>
{
    using type = int16_t;
};

template<>
struct value_helper<value_type::uint16_type>
{
    using type = uint16_t;
};

template<>
struct value_helper<value_type::int32_type>
{
    using type = int32_t;
};

template<>
struct value_helper<value_type::uint32_type>
{
    using type = uint32_t;
};

template<>
struct value_helper<value_type::int64_type>
{
    using type = int64_t;
};

template<>
struct value_helper<value_type::uint64_type>
{
    using type = uint64_t;
};

template<>
struct value_helper<value_type::boolean_type>
{
    using type = bool;
};

template<>
struct value_helper<value_type::binary_type>
{
    using type = std::vector<uint8_t>;
};

template<>
struct value_helper<value_type::guid_type>
{
    using type = guid;
};

template<>
struct value_helper<value_type::filetime_type>
{
    using type = std::pair<uint64_t, uint64_t>;
};

template<>
struct value_helper<value_type::systemtime_type>
{
    using type = std::array<uint16_t, 8>;
};

template<>
struct value_helper<value_type::sid_type>
{
    using type = sid;
};

template<>
struct value_helper<value_type::hex32_type>
{
    using type = hex32;
};

template<>
struct value_helper<value_type::hex64_type>
{
    using type = hex64;
};

template<>
struct value_helper<value_type::wstring_array_type>
{
    using type = std::vector<std::wstring>;
};

struct value_spec
{
    value_type type;
    uint16_t size;
};

class chunk;

class value
{
public:
    typedef boost::variant<
			    value_helper<value_type::null_type>::type,
			    value_helper<value_type::wstring_type>::type,
			    value_helper<value_type::int8_type>::type, 
			    value_helper<value_type::uint8_type>::type,
			    value_helper<value_type::int16_type>::type,
			    value_helper<value_type::uint16_type>::type,
			    value_helper<value_type::int32_type>::type,
			    value_helper<value_type::uint32_type>::type,
			    value_helper<value_type::int64_type>::type,
			    value_helper<value_type::uint64_type>::type,
			    value_helper<value_type::boolean_type>::type,
			    value_helper<value_type::binary_type>::type,
			    value_helper<value_type::guid_type>::type,
			    value_helper<value_type::filetime_type>::type,
			    value_helper<value_type::systemtime_type>::type,
			    value_helper<value_type::sid_type>::type,
			    value_helper<value_type::hex32_type>::type,
			    value_helper<value_type::hex64_type>::type,
			    value_helper<value_type::wstring_array_type>::type> data_type;

    operator std::string() const;

    bool empty() const;
    value_type type() const;
    std::string str() const;

private:
    void init(value_type type);
    void read(std::istream &stream, const value_spec &vs);
    void read(std::istream &stream, value_type type, uint16_t size);

    value_type type_;
    data_type data;

friend class binxml_parser;
};

} // namespace pevtx

#endif //PEVTX_VALUE_HPP_INCLUDED
