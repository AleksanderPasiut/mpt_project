///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters_buffer.hpp"
#include "qr_code.hpp"

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

    ParametersBuffer m_params {};

    generate_qr_code_bmp(port);

    ServerFileApplication fsapp { port, "../website" };

    fsapp.get_impl().set_default_path("index.html");

    fsapp.get_impl().register_on_post_handler("/initialize", [](const std::string_view& contents, const std::string_view& cookie) -> CustomResponse
    {
        std::cout << "initialize\n";
        return CustomResponse(200, "text/plain", "", "DummyCookieValue");
    });

    fsapp.get_impl().register_custom_handler("/values.txt",
        std::bind(&ParametersBuffer::handle_parameters_get, &m_params, std::placeholders::_1, std::placeholders::_2) );

    fsapp.get_impl().register_custom_handler("/value",
        std::bind(&ParametersBuffer::handle_parameter_set, &m_params, std::placeholders::_1, std::placeholders::_2) );

    fsapp.get_impl().register_custom_handler("/qr_code.bmp", get_qr_code);
    
    fsapp.get_impl().register_on_post_handler("/string0",
        std::bind(&ParametersBuffer::set_string<0>, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    fsapp.get_impl().register_on_post_handler("/string1",
        std::bind(&ParametersBuffer::set_string<1>, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    fsapp.get_impl().register_on_post_handler("/string2",
        std::bind(&ParametersBuffer::set_string<2>, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    fsapp.get_impl().register_on_post_handler("/trigger",
        std::bind(&ParametersBuffer::compute, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    fsapp.get_impl().register_custom_handler("/string.txt",
        std::bind(&ParametersBuffer::get_string_output, &m_params, std::placeholders::_1, std::placeholders::_2) );

    fsapp.run();

    return 0;
}
