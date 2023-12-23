///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <stdexcept>

#include <stdio.h>
#include <unistd.h>
#include <poll.h>

class StreamWrapper
{
public:
    StreamWrapper(const std::string& command) : m_stream( popen(command.c_str(), "r") )
    {
        if (!m_stream)
        {
            throw std::logic_error("Failed to start a process!");
        }
    }

    StreamWrapper(const StreamWrapper&) = delete;
    StreamWrapper& operator= (const StreamWrapper&) = delete;

    ~StreamWrapper() noexcept
    {
        pclose(m_stream);
        m_stream = NULL;
    }

    int get_fd()
    {
        return fileno(m_stream);
    }

    void read(std::string& output, size_t max_size)
    {
        output.resize(max_size);
        const size_t write_size = fread(&output[0], 1, max_size, m_stream);
        output.resize(write_size);
    }

private:
    FILE* m_stream {};
};
