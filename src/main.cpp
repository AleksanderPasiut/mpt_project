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

class SessionIdGenerator
{
public:
    std::string get()
    {
        std::stringstream ss {};
        ss << "SessionIdDummy_" << m_counter;
        ++m_counter;
        return ss.str();
    }

private:
    size_t m_counter {};
};

class ParametersBufferMulticlient
{
public:
    using Func = CustomResponse(ParametersBuffer::*)(const std::string_view& query, const std::string_view& cookie);

    ParametersBufferMulticlient()
    {}

    std::string initialize()
    {
        std::string const cookie_data = m_session_id_generator.get();

        m_buffer[cookie_data] = ParametersBuffer();

        return cookie_data;
    }

    CustomResponse invoke_buffer_function(const std::string_view& query, const std::string_view& cookie, Func func)
    {
        const std::string session_id = std::string(cookie);

        auto it = m_buffer.find(session_id);
        if (it != m_buffer.end())
        {
            std::cout << __func__ << " " << session_id << '\n';
            
            ParametersBuffer& parameters_buffer = it->second;
            return std::invoke(func, &parameters_buffer, query, cookie);
        }
        else
        {
            std::cerr << "Unknown session id! " << session_id << "\n";
            return CustomResponse(500);
        }
    }

private:
    SessionIdGenerator m_session_id_generator {};

    std::unordered_map<std::string, ParametersBuffer> m_buffer {};
};

int main(int argc, char* argv[])
{
    const std::string port = process_args(argc, argv);

    ParametersBufferMulticlient m_parameters_buffer_multiclient {};
    // ParametersBuffer m_params {};

    generate_qr_code_bmp(port);

    ServerFileApplication fsapp { port, "../website" };

    fsapp.get_impl().set_default_path("index.html");

    fsapp.get_impl().register_custom_handler("/qr_code.bmp", get_qr_code);


    fsapp.get_impl().register_on_post_handler("/initialize", [&m_parameters_buffer_multiclient](const std::string_view& contents, const std::string_view& cookie) -> CustomResponse
    {
        std::cout << "initialize\n";
        std::string const session_id = m_parameters_buffer_multiclient.initialize();
        return CustomResponse(200, "text/plain", "", session_id);
    });

    auto register_custom_handler_helper = [&fsapp, &m_parameters_buffer_multiclient](std::string&& path, ParametersBufferMulticlient::Func func)
    {
        fsapp.get_impl().register_custom_handler(path,
            std::bind(&ParametersBufferMulticlient::invoke_buffer_function, &m_parameters_buffer_multiclient, std::placeholders::_1, std::placeholders::_2, func));
    };

    auto register_on_post_handler_helper = [&fsapp, &m_parameters_buffer_multiclient](std::string&& path, ParametersBufferMulticlient::Func func)
    {
        fsapp.get_impl().register_on_post_handler(path,
            std::bind(&ParametersBufferMulticlient::invoke_buffer_function, &m_parameters_buffer_multiclient, std::placeholders::_1, std::placeholders::_2, func));
    };

    register_custom_handler_helper("/values.txt",
        &ParametersBuffer::handle_parameters_get);

    register_custom_handler_helper("/value",
        &ParametersBuffer::handle_parameter_set);

    register_custom_handler_helper("/string.txt",
        &ParametersBuffer::get_string_output);
        
    register_on_post_handler_helper("/string0",
        &ParametersBuffer::set_string<0>);

    register_on_post_handler_helper("/string1",
        &ParametersBuffer::set_string<1>);
    
    register_on_post_handler_helper("/string2",
        &ParametersBuffer::set_string<2>);
    
    register_on_post_handler_helper("/triggers",
        &ParametersBuffer::compute);


    // fsapp.get_impl().register_custom_handler("/values.txt",
    //     std::bind(&ParametersBufferMulticlient::invoke_buffer_function, &m_parameters_buffer_multiclient, std::placeholders::_1, std::placeholders::_2, &ParametersBuffer::handle_parameters_get));


    // fsapp.get_impl().register_custom_handler("/values.txt",
    //     std::bind(&ParametersBuffer::handle_parameters_get, &m_params, std::placeholders::_1, std::placeholders::_2) );

    // fsapp.get_impl().register_custom_handler("/value",
    //     std::bind(&ParametersBuffer::handle_parameter_set, &m_params, std::placeholders::_1, std::placeholders::_2) );

    
    // fsapp.get_impl().register_on_post_handler("/string0",
    //     std::bind(&ParametersBuffer::set_string<0>, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    // fsapp.get_impl().register_on_post_handler("/string1",
    //     std::bind(&ParametersBuffer::set_string<1>, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    // fsapp.get_impl().register_on_post_handler("/string2",
    //     std::bind(&ParametersBuffer::set_string<2>, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    // fsapp.get_impl().register_on_post_handler("/trigger",
    //     std::bind(&ParametersBuffer::compute, &m_params, std::placeholders::_1, std::placeholders::_2) );
    
    // fsapp.get_impl().register_custom_handler("/string.txt",
    //     std::bind(&ParametersBuffer::get_string_output, &m_params, std::placeholders::_1, std::placeholders::_2) );

    fsapp.run();

    return 0;
}
