#include <iostream>
#include <pevtx/evtx_parser.hpp>

int main(int argc, char *argv[])
{
    try
    {
	if(argc == 2)
	{
	    pevtx::evtx evtx;
	    pevtx::evtx_parser parser;
	    parser.parse(argv[1], evtx);
	}
	else std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    }
    catch(const std::exception &ex)
    {
	std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
