///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <stdexcept>

#include <stdio.h>

class Process
{
public:
    Process(const std::string& command)
        : m_stream( popen(command.c_str(), "r") )
    {
        if (!m_stream)
        {
            throw std::logic_error("Failed to start a process");
        }
    }

    Process(const Process&) = delete;
    Process& operator= (const Process&) = delete;

    ~Process() noexcept
    {
        pclose(m_stream);
        m_stream = NULL;
    }

    std::string readsome(size_t max_size)
    {
        std::string ret {};
        ret.resize(max_size);
        size_t read_count = fread( &ret[0], 1, max_size, m_stream);
        ret.resize(read_count);
        return ret;
    }

private:
    FILE* m_stream {};
};
