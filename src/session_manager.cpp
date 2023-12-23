///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "session_manager.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

std::string SessionManager::initialize()
{
    std::string const cookie_data = m_session_id_generator.get();

    m_buffer[cookie_data] = Session();

    return cookie_data;
}

CustomResponse SessionManager::invoke_session_function(const std::string_view& query, const std::string_view& cookie, Func func)
{
    const std::string session_id = std::string(cookie);

    auto it = m_buffer.find(session_id);
    if (it != m_buffer.end())
    {
        std::cout << __func__ << " " << session_id << '\n';

        Session& parameters_buffer = it->second;
        return std::invoke(func, &parameters_buffer, query, cookie);
    }
    else
    {
        std::cerr << "Unknown session id! " << session_id << "\n";
        return CustomResponse(500);
    }
}
