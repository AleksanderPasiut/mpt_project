///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "session_manager.hpp"
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

    SessionManager session_manager {};

    generate_qr_code_bmp(port);

    ServerFileApplication fsapp { port, "../website" };

    fsapp.get_impl().set_default_path("index.html");

    fsapp.get_impl().register_custom_handler("/qr_code.bmp", get_qr_code);

    fsapp.get_impl().register_on_post_handler("/initialize", [&session_manager](const std::string_view& contents, const std::string_view& cookie) -> CustomResponse
    {
        std::cout << "initialize\n";
        std::string const session_id = session_manager.initialize();
        return CustomResponse(200, "text/plain", "", session_id);
    });

    auto register_custom_handler_helper = [&fsapp, &session_manager](std::string&& path, SessionManager::Func func)
    {
        fsapp.get_impl().register_custom_handler(path,
            std::bind(&SessionManager::invoke_session_function, &session_manager, std::placeholders::_1, std::placeholders::_2, func));
    };

    auto register_on_post_handler_helper = [&fsapp, &session_manager](std::string&& path, SessionManager::Func func)
    {
        fsapp.get_impl().register_on_post_handler(path,
            std::bind(&SessionManager::invoke_session_function, &session_manager, std::placeholders::_1, std::placeholders::_2, func));
    };

    register_custom_handler_helper("/values.txt",
        &Session::handle_parameters_get);

    register_custom_handler_helper("/value",
        &Session::handle_parameter_set);

    register_custom_handler_helper("/string.txt",
        &Session::get_string_output);
        
    register_on_post_handler_helper("/string0",
        &Session::set_string<0>);

    register_on_post_handler_helper("/string1",
        &Session::set_string<1>);
    
    register_on_post_handler_helper("/string2",
        &Session::set_string<2>);
    
    register_on_post_handler_helper("/trigger",
        &Session::compute);

    fsapp.run();

    return 0;
}
