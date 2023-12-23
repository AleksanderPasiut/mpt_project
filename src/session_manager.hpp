///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "session.hpp"
#include "session_id_generator.hpp"

#include <string>
#include <unordered_map>
#include <chrono>

class SessionManager
{
public:
    using Func = CustomResponse(Session::*)(const std::string_view& query);
    using TimePoint = std::chrono::system_clock::time_point;
    using Duration = std::chrono::system_clock::duration;

    SessionManager() = default;
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator= (const SessionManager&) = delete;

    std::string initialize();

    CustomResponse invoke_session_function(const std::string_view& query, const std::string_view& cookie, Func func);

private:
    void clean_stalled_sessions(const TimePoint& now);

    SessionIdGenerator m_session_id_generator {};

    struct SessionData
    {
        SessionData(std::string id);

        Session session;
        TimePoint last_invoke_timepoint;
    };

    std::unordered_map<std::string, SessionData> m_buffer {};
};
