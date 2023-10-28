///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters.hpp"

#include "server/server.file_application.hpp"

#include <iostream>
#include <string>

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

    Params m_params {};

    ServerFileApplication fsapp { port, "../website" };

    fsapp.set_default_path("index.html");

    fsapp.register_custom_handler("/values.txt", std::bind(&Params::handle_parameters_get, &m_params, std::placeholders::_1) );
    fsapp.register_custom_handler("/value", std::bind(&Params::handle_parameter_set, &m_params, std::placeholders::_1) );
    fsapp.run();

    return 0;
}
