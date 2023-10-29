///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters.hpp"

#include <regex>
#include <sstream>

#include <qr_gen.hpp>

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

CustomResponse Params::set_string_parameter(const std::string_view& query)
{
    if (query.length() > 0)
    {
        m_string_parameter = query;

        run_qr_gen(m_string_parameter.data(), "qr_code.bmp");

        return CustomResponse(200);
    }
    
    return CustomResponse(500);
}

CustomResponse Params::get_string_parameter(const std::string_view&)
{
    return CustomResponse(200, "text/plain", m_string_parameter);
}
