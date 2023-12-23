///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "session.hpp"
#include "session_id_generator.hpp"

#include <string>
#include <unordered_map>

class SessionManager
{
public:
    using Func = CustomResponse(Session::*)(const std::string_view& query, const std::string_view& cookie);

    SessionManager()
    {}

    std::string initialize();

    CustomResponse invoke_session_function(const std::string_view& query, const std::string_view& cookie, Func func);

private:
    SessionIdGenerator m_session_id_generator {};

    std::unordered_map<std::string, Session> m_buffer {};
};
