///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "session_manager.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <chrono>

std::string SessionManager::initialize()
{
    std::string&& cookie_data = m_session_id_generator.get();

    m_buffer.emplace( cookie_data, SessionData() );

    return cookie_data;
}

CustomResponse SessionManager::invoke_session_function(const std::string_view& query, const std::string_view& cookie, Func func)
{
    const std::string session_id = std::string(cookie);

    TimePoint now = std::chrono::system_clock::now();
    clean_stalled_sessions(now);

    auto it = m_buffer.find(session_id);
    if (it != m_buffer.end())
    {
        std::cout << __func__ << " " << session_id << '\n';

        it->second.last_invoke_timepoint = now;

        Session& session = it->second.session;
        return std::invoke(func, &session, query);
    }
    else
    {
        std::cerr << "Unknown session id! " << session_id << "\n";
        return CustomResponse(500);
    }
}

void SessionManager::clean_stalled_sessions(const TimePoint& now)
{
    const Duration timeout = std::chrono::seconds(2);

    using It = decltype(m_buffer)::iterator;
    std::vector<It> stalled_sessions {};
    stalled_sessions.reserve(m_buffer.size());

    for (auto it = m_buffer.begin(); it != m_buffer.end(); ++it)
    {
        const Duration diff = now - it->second.last_invoke_timepoint;

        if (diff > timeout)
        {
            stalled_sessions.push_back(it);            
        }
    }

    for (It& it : stalled_sessions)
    {
        std::cout << "Cleaning session " << it->first << '\n';
        m_buffer.erase(it);
    }
}

SessionManager::SessionData::SessionData()
    : session()
    , last_invoke_timepoint( std::chrono::system_clock::now() )
{}
