#include "pch.h"

#include <Server.h>

int main(int argc, const char** argv)
{
    try
    {
        ProxyLib::Server server(50000);

        server.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception" << std::endl;

        return EXIT_FAILURE;
    }
	
    return EXIT_SUCCESS;
}

