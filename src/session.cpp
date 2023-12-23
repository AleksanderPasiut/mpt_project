///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "session.hpp"

#include <regex>
#include <sstream>

CustomResponse Session::handle_parameter_set(const std::string_view& query)
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

CustomResponse Session::handle_parameters_get(const std::string_view&)
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

CustomResponse Session::get_string_output(const std::string_view&)
{
    std::cout << __func__ << '\n';

    if (m_capd_process_ptr.get())
    {
        switch (m_capd_process_ptr->get_state())
        {
            case State::Ready:
            {
                m_string_output = m_capd_process_ptr->get_and_clear_output();
                m_capd_process_ptr.reset();
                break;
            }
            case State::Waiting:
            {
                m_string_output = "Computing...";
                break;
            }
            case State::ErrorTimeout:
            {
                m_string_output = "Computation timeout reached!";
                m_capd_process_ptr.reset();
                break;
            }
            default:
            {
                m_string_output = "Unknown computation error!";
                m_capd_process_ptr.reset();
                break;
            }
        }
    }

    return CustomResponse(200, "text/plain", m_string_output);
}

CustomResponse Session::compute(const std::string_view&)
{
    std::cout << __func__ << '\n';

    try
    {
        if (m_capd_process_ptr.get() == nullptr)
        {
            m_capd_process_ptr = std::make_unique<CapdProcess>(
                CapdProcessParams(
                    m_string_parameter[0],
                    m_string_parameter[1],
                    m_string_parameter[2],
                    m_buffer[0],
                    m_buffer[1]
                )
            );
        }
        else
        {
            throw std::logic_error("Busy");
        }
    }
    catch (const std::exception& e)
    {
        m_string_output = std::string("Computation failed: ") + e.what();
    }
    catch (...)
    {
        m_string_output = std::string("Computation failed due to unknown exception!");
    }

    return CustomResponse(200);
}

