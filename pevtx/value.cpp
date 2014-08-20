#include <iomanip>
#include <boost/locale.hpp>
#include <pevtx/value.hpp>
#include <pevtx/reader.hpp>
#include <pevtx/binxml.hpp>

namespace pevtx
{

struct reader_visitor : public boost::static_visitor<>, public reader
{
    reader_visitor(std::istream &stream, std::size_t s) 
	: reader(stream),
	  in(stream),
	  size(s) 
    {

    }

    template<typename T>
    void operator()(T &value) const {read(value);}

    void operator()(value_helper<value_type::null_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::wstring_type>::type &data) const {read(data, size);}
    void operator()(value_helper<value_type::binary_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::guid_type>::type &data) const {read(data);}
    void operator()(value_helper<value_type::filetime_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::systemtime_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::sid_type>::type &data) const {skip(size);}
    void operator()(value_helper<value_type::wstring_array_type>::type &data) const {skip(size);}

    std::istream &in;
    std::size_t size;
};

struct str_visitor : public boost::static_visitor<std::string>
{
    template<typename T>
    std::string operator()(const T &data) const {return std::to_string(data);}

    std::string operator()(const value_helper<value_type::null_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::wstring_type>::type &data) const 
    {
	return boost::locale::conv::utf_to_utf<char, wchar_t>(data.c_str(), data.c_str() + data.size());
    }

    std::string operator()(const value_helper<value_type::boolean_type>::type &data) const {return (data ? "true" : "false");}

    std::string operator()(const value_helper<value_type::binary_type>::type &data) const {return std::string();}

    std::string operator()(const value_helper<value_type::guid_type>::type &data) const 
    {
	std::stringstream ss;
	ss << std::hex << '{'	<< data.data1 << '-' 
				<< data.data2 << '-'
				<< data.data3 << '-'
				<< std::setfill('0') << std::setw(2) << static_cast<unsigned>(data.data4[0]) 
				<< std::setfill('0') << std::setw(2) << static_cast<unsigned>(data.data4[1]) << '-';

	for(size_t i = 2; i < data.data4.size(); ++i) ss << std::setfill('0') << std::setw(2) << static_cast<unsigned>(data.data4[i]);
	ss << '}';
	return ss.str();
    }

    std::string operator()(const value_helper<value_type::filetime_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::systemtime_type>::type &data) const {return std::string();}
    std::string operator()(const value_helper<value_type::sid_type>::type &data) const {return std::string();}


    std::string operator()(const value_helper<value_type::hex32_type>::type &data) const 
    {
	return data.str();
    }

    std::string operator()(const value_helper<value_type::hex64_type>::type &data) const
    {
	return data.str();
    }

    std::string operator()(const value_helper<value_type::wstring_array_type>::type &data) const {return std::string();}
};

value::operator std::string() const
{
    return str();
}

bool value::empty() const
{
    return (type_ == value_type::null_type);
}

value_type value::type() const
{
    return type_;
}

std::string value::str() const
{
    return boost::apply_visitor(str_visitor(), data);
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
	VALUE_INIT_CASE(value_type::boolean_type);
	VALUE_INIT_CASE(value_type::binary_type);
	VALUE_INIT_CASE(value_type::guid_type);
	VALUE_INIT_CASE(value_type::filetime_type);
	VALUE_INIT_CASE(value_type::systemtime_type);
	VALUE_INIT_CASE(value_type::sid_type);
	VALUE_INIT_CASE(value_type::hex32_type);
	VALUE_INIT_CASE(value_type::hex64_type);
	VALUE_INIT_CASE(value_type::wstring_array_type);
	default: 
std::cout << std::hex << static_cast<uint16_t>(type) << std::endl;
	    throw std::runtime_error("Bad type!");
    }
    type_ = type;
}

void value::read(std::istream &stream, const value_spec &vs)
{
    init(vs.type);
    boost::apply_visitor(reader_visitor(stream, vs.size), data);
}

void value::read(std::istream &stream, value_type type, uint16_t size)
{
    init(type);
    boost::apply_visitor(reader_visitor(stream, size), data);
}

} // namespace pevtx
