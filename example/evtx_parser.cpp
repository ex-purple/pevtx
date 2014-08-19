#include <iostream>
#include <pevtx/evtx_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

int main(int argc, char *argv[])
{
    try
    {
	if(argc == 2)
	{
	    pevtx::evtx evtx;
	    pevtx::evtx_parser parser;
	    parser.parse(argv[1], evtx);

	    for(auto &chunk:evtx.get_chunks())
	    {
		std::cout << "Chunk" << std::endl;
		for(auto &record:chunk.get_records())
		{
		    std::cout << "Record id: " << record.get_id() << std::endl;

		    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
		    boost::property_tree::write_xml(std::cout, record.get_root(), settings);
		    std::cout << std::endl;

		    std::cout << std::endl;
		}
		std::cout << std::endl;
	    }

	}
	else std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    }
    catch(const std::exception &ex)
    {
	std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
