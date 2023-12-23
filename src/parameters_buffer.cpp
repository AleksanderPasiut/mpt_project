///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "parameters_buffer.hpp"
#include "capd_process.hpp"

#include <regex>
#include <sstream>

CustomResponse ParametersBuffer::handle_parameter_set(const std::string_view& query, const std::string_view& cookie)
{
    std::cout << __func__ << ' ' << query << '\n';

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

CustomResponse ParametersBuffer::handle_parameters_get(const std::string_view&, const std::string_view& cookie)
{
    std::cout << __func__ << '\n';

    std::stringstream ss {};

    auto it = m_buffer.begin();
    ss << *it;
    for (++it; it != m_buffer.end(); ++it)
    {
        ss << ";" << *it;
    }

    return CustomResponse(200, "text/plain", ss.str());
}

CustomResponse ParametersBuffer::get_string_output(const std::string_view&, const std::string_view& cookie)
{
    std::cout << __func__ << '\n';

    return CustomResponse(200, "text/plain", m_string_output);
}

CustomResponse ParametersBuffer::compute(const std::string_view&, const std::string_view& cookie)
{
    std::cout << __func__ << '\n';

    try
    {
        CapdProcess capd_process
        {
            CapdProcessParams(
                m_string_parameter[0],
                m_string_parameter[1],
                m_string_parameter[2],
                m_buffer[0],
                m_buffer[1]
            )
        };

        m_string_output = capd_process.get_resp();
    }
    catch (const std::exception& e)
    {
        m_string_output = std::string("Computation failed: ") + e.what();
    }
    catch (...)
    {
        m_string_output = std::string("Computation failed due to unknown exception");
    }

    return CustomResponse(200);
}

