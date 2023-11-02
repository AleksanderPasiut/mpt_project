///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters.hpp"
#include "capd_process.hpp"

#include <regex>
#include <sstream>

CustomResponse Params::handle_parameter_set(const std::string_view& query)
{
    std::regex re( R"(^input([0-9]*)=([0-9\.]*)$)");
    std::match_results<std::string_view::iterator> res {};
    if ( std::regex_match(query.begin(), query.end(), res, re) )
    {
        if (res.size() == 3)
        {
            size_t idx = std::stoi( res[1].str() );
            double value = std::stod( res[2].str() );

            if (idx < m_buffer.size())
            {
                m_buffer[idx] = value;
            }

            return CustomResponse(200);
        }
    }

    return CustomResponse(500);
}

CustomResponse Params::handle_parameters_get(const std::string_view&)
{
    std::stringstream ss {};

    auto it = m_buffer.begin();
    ss << *it;
    for (++it; it != m_buffer.end(); ++it)
    {
        ss << ";" << *it;
    }

    return CustomResponse(200, "text/plain", ss.str());
}

CustomResponse Params::get_string_output(const std::string_view&)
{
    return CustomResponse(200, "text/plain", m_string_output);
}

CustomResponse Params::compute(const std::string_view&)
{
    try
    {
        CapdProcess capd_process
        {
            CapdProcess::Params
            {
                .executable_path = "backend/capd_backend",
                .func_str = m_string_parameter[0], //"var:x,y;fun:x+y,x-y;",
                .initial_condition_values_str = m_string_parameter[1], // "1.0;2.0;"
                .integration_time = std::stod( m_string_parameter[2] ),
                .order = m_buffer[0],
                .decimal_places = m_buffer[1]
            }
        };

        m_string_output = capd_process.get_resp();
    }
    catch (std::exception e)
    {
        m_string_output = std::string("Computation failed: ") + e.what();
    }
    catch (...)
    {
        m_string_output = std::string("Computation failed due to unknown exception");
    }

    return CustomResponse(200);
}

