///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "parameters_buffer.hpp"
#include "session_id_generator.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

class ParametersBufferMulticlient
{
public:
    using Func = CustomResponse(ParametersBuffer::*)(const std::string_view& query, const std::string_view& cookie);

    ParametersBufferMulticlient()
    {}

    std::string initialize()
    {
        std::string const cookie_data = m_session_id_generator.get();

        m_buffer[cookie_data] = ParametersBuffer();

        return cookie_data;
    }

    CustomResponse invoke_buffer_function(const std::string_view& query, const std::string_view& cookie, Func func)
    {
        const std::string session_id = std::string(cookie);

        auto it = m_buffer.find(session_id);
        if (it != m_buffer.end())
        {
            std::cout << __func__ << " " << session_id << '\n';

            ParametersBuffer& parameters_buffer = it->second;
            return std::invoke(func, &parameters_buffer, query, cookie);
        }
        else
        {
            std::cerr << "Unknown session id! " << session_id << "\n";
            return CustomResponse(500);
        }
    }

private:
    SessionIdGenerator m_session_id_generator {};

    std::unordered_map<std::string, ParametersBuffer> m_buffer {};
};
