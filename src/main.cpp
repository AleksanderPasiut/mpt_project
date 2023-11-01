///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters.hpp"
#include "process.hpp"
#include "capd_process.hpp"

#include "server/server.file_application.hpp"

#include <qr_gen.hpp>

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

static void generate_qr_code_bmp(const std::string& port)
{
    Process hostname_cmd("hostname -I");
    const std::string hostname_cmd_resp = hostname_cmd.readsome(1024);

    std::regex re { R"(([0-9\.]*))" };
    std::smatch res {};
    if (std::regex_search(hostname_cmd_resp, res, re))
    {
        if (res.size() == 2)
        {
            const std::string hostname = res[1];
            const std::string url = "http://" + hostname + ":" + port;
            run_qr_gen(url.c_str(), "qr_code.bmp");
        }
    }
}

void execute_capd_process()
{
    CapdProcess capd_process
    {
        CapdProcess::Params
        {
            .executable_path = "backend/capd_backend",
            .func_str = "var:x,y;fun:x+y,x-y;",
            .initial_condition_values_str = "1.0;2.0",
            .integration_time = 1.0,
            .order = 20,
            .decimal_places = 20
        }
    };

    std::cout << capd_process.get_resp() << '\n';
}

int main(int argc, char* argv[])
{
    execute_capd_process();    

    const std::string port = process_args(argc, argv);

    Params m_params {};

    generate_qr_code_bmp(port);

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
