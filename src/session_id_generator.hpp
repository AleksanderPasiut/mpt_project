///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>
#include <sstream>

class SessionIdGenerator
{
public:
    SessionIdGenerator() = default;
    SessionIdGenerator(const SessionIdGenerator&) = delete;
    SessionIdGenerator& operator= (const SessionIdGenerator&) = delete;

    std::string get()
    {
        std::stringstream ss {};
        ss << "SessionIdDummy_" << m_counter;
        ++m_counter;
        return ss.str();
    }

private:
    size_t m_counter {};
};
