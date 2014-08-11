#include <boost/locale.hpp>
#include <pevtx/value.hpp>
#include <pevtx/reader.hpp>

namespace pevtx
{

struct reader_visitor : public boost::static_visitor<>, public reader
{
    reader_visitor(std::size_t s, std::istream &stream) : reader(stream), size(s) {}

    template<typename T>
    void operator()(T &value) const {read(value);}

    void operator()(value_helper<value_type::null_type>::type &data) const {}
    void operator()(value_helper<value_type::wstring_type>::type &data) const {read(data, size);}
    void operator()(value_helper<value_type::guid_type>::type &data) const {read(data);}
    void operator()(value_helper<value_type::filetime_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::hex64_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::bxml_type>::type &data) const {skip(size);}

    std::size_t size;
};

struct str_visitor : public boost::static_visitor<std::string>
{
    template<typename T>
    std::string operator()(const T &data) const {return std::to_string(data);}

    std::string operator()(const value_helper<value_type::null_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::wstring_type>::type &data) const {return boost::locale::conv::utf_to_utf<char, wchar_t>(data.c_str(), data.c_str() + data.size());}
    std::string operator()(const value_helper<value_type::guid_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::filetime_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::hex64_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::bxml_type>::type &data) const {return std::string();}
};

value::operator std::string() const
{
    return boost::apply_visitor(str_visitor(), data);
}

bool value::empty() const
{
    return (type_ == value_type::null_type);
}

value_type value::type() const
{
    return type_;
}

void value::read(const value_spec &vs, std::istream &stream)
{
    init(vs.type);
    boost::apply_visitor(reader_visitor(vs.size, stream), data);
}

void value::read(value_type type, uint16_t size, std::istream &stream)
{
    init(type);
    boost::apply_visitor(reader_visitor(size, stream), data);
}

#define VALUE_INIT_CASE(t) \
    case t: \
	data = value_helper<t>::type(); \
	break;

void value::init(value_type type)
{
    switch(type)
    {
        VALUE_INIT_CASE(value_type::null_type);
	VALUE_INIT_CASE(value_type::wstring_type);
        VALUE_INIT_CASE(value_type::int8_type);
        VALUE_INIT_CASE(value_type::uint8_type);
        VALUE_INIT_CASE(value_type::int16_type);
        VALUE_INIT_CASE(value_type::uint16_type);
	VALUE_INIT_CASE(value_type::int32_type);
        VALUE_INIT_CASE(value_type::uint32_type);
	VALUE_INIT_CASE(value_type::int64_type);
        VALUE_INIT_CASE(value_type::uint64_type);
	VALUE_INIT_CASE(value_type::guid_type);
	VALUE_INIT_CASE(value_type::filetime_type);
	VALUE_INIT_CASE(value_type::hex64_type);
	VALUE_INIT_CASE(value_type::bxml_type);
	default: 
	    throw std::runtime_error("Bad type!");
    }
    type_ = type;
}

} // namespace pevtx
