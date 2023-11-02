///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters.hpp"
#include "process.hpp"
#include "capd_process.hpp"

#include "server/server.file_application.hpp"


#include <iostream>
#include <string>
#include <fstream>
#include <regex>

static std::string process_args(int argc, char* argv[])
{
    if (argc == 2)
    {
        return std::string( argv[1] );
    }
    
    return std::string("3490");
}

static CustomResponse get_qr_code(const std::string_view& query)
{
    std::ifstream fs("qr_code.bmp", std::ifstream::binary);
    if (fs)
    {
        std::string buffer {};
        buffer.resize(65536);
        size_t n = fs.readsome(&buffer[0], buffer.size());
        buffer.resize(n);
        return CustomResponse(200, "image/bmp", buffer);
    }

    return CustomResponse(404);
}

int main(int argc, char* argv[])
{
    const std::string port = process_args(argc, argv);

    Params m_params {};

    ServerFileApplication fsapp { port, "../website" };

    fsapp.set_default_path("index.html");

    fsapp.register_custom_handler("/values.txt", std::bind(&Params::handle_parameters_get, &m_params, std::placeholders::_1) );
    fsapp.register_custom_handler("/value", std::bind(&Params::handle_parameter_set, &m_params, std::placeholders::_1) );
    fsapp.register_custom_handler("/qr_code.bmp", get_qr_code);
    fsapp.register_on_post_handler("/string", std::bind(&Params::set_string_parameter, &m_params, std::placeholders::_1) );
    fsapp.register_custom_handler("/string.txt", std::bind(&Params::get_string_parameter, &m_params, std::placeholders::_1) );
    fsapp.run();

    return 0;
}
