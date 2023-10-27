///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include "server/server.file_application.hpp"

static std::string process_args(int argc, char* argv[])
{
    if (argc == 2)
    {
        return std::string( argv[1] );
    }
    
    return std::string("3490");
}

int main(int argc, char* argv[])
{
    const std::string port = process_args(argc, argv);

    ServerFileApplication fsapp { port, "../website" };

    fsapp.set_default_path("index.html");
    fsapp.run();

    return 0;
}
