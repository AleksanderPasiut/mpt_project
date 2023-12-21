///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "types.hpp"

#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Unix socket wrapper (specialized for HTTP communication endpoint accessing)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SocketInternal
{
public:
    SocketInternal(Addrinfo* addrinfo) : m_addrinfo(addrinfo)
    {
        m_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

        if (m_fd == -1)
        {
            throw std::logic_error("Failed to make a socket!");
        }
    }

    SocketInternal(const SocketInternal&) = delete;
    SocketInternal& operator= (const SocketInternal&) = delete;

    SocketInternal(SocketInternal&& arg)
    {
        this->m_fd = arg.m_fd;
        arg.m_fd = -1;
    }

    virtual ~SocketInternal() noexcept
    {
        if (m_fd != -1)
        {
            close(m_fd);
            m_fd = -1;
        }
    }

    void set_reuse_addr()
    {
        int yes = 1;
        if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            throw std::logic_error("Failed to set socket option!");
        }
    }

    void bind()
    {
        if (::bind(m_fd, m_addrinfo->ai_addr, m_addrinfo->ai_addrlen) == -1)
        {
            throw std::logic_error("Failed to bind to socket!");
        }

    }

    void listen()
    {
        if (::listen(m_fd, MAX_CONNECTION_REQUESTS) == -1)
        {
            throw std::logic_error("Failed to listen to socket!");
        }
    }

    int get_fd() const
    {
        return m_fd;
    }

private:
    static const int MAX_CONNECTION_REQUESTS = 10;

    Addrinfo * m_addrinfo;
    int m_fd;
};
