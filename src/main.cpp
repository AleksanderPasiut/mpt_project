///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include "server/server.file_application.hpp"


#include <regex>
#include <string_view>
#include <array>
#include <sstream>

struct ParamsX
{
    std::array<unsigned, 5> m_buffer {};
};

ParamsX m_params {};

static std::string process_args(int argc, char* argv[])
{
    if (argc == 2)
    {
        return std::string( argv[1] );
    }
    
    return std::string("3490");
}

CustomResponse handle_parameter_set(const std::string_view& query)
{
    std::regex re( R"(^input([0-9]*)=([0-9\.]*)$)");
    std::match_results<std::string_view::iterator> res {};
    if ( std::regex_match(query.begin(), query.end(), res, re) )
    {
        if (res.size() == 3)
        {
            size_t idx = std::stoi( res[1].str() );
            double value = std::stod( res[2].str() );

            if (idx < m_params.m_buffer.size())
            {
                m_params.m_buffer[idx] = value;
            }

            return CustomResponse(200);
        }
    }

    return CustomResponse(500);
}

CustomResponse handle_parameters_get(const std::string_view&)
{
    std::stringstream sst {};

    auto it = m_params.m_buffer.begin();
    sst << *it;
    for (++it; it != m_params.m_buffer.end(); ++it)
    {
        sst << ";" << *it;
    }

    return CustomResponse(200, "text/plain", sst.str());
}

int main(int argc, char* argv[])
{
    const std::string port = process_args(argc, argv);

    ServerFileApplication fsapp { port, "../website" };

    fsapp.set_default_path("index.html");

    fsapp.register_custom_handler("/values.txt", handle_parameters_get);
    fsapp.register_custom_handler("/value", handle_parameter_set);
    fsapp.run();

    return 0;
}
