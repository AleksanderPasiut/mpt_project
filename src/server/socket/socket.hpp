///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <stdexcept>

#include "serv_addrinfo.hpp"
#include "socket_internal.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Unix socket wrapper (specialized for HTTP communication endpoint accessing)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Socket
{
public:
    Socket(const std::string_view& port) : m_socket_internal( get_listener_socket(port.data()) )
    {}

    Socket(const Socket&) = delete;
    Socket& operator= (const Socket&) = delete;

    int get_fd() const
    {
        return m_socket_internal.get_fd();
    }

private:
    static SocketInternal get_listener_socket(const char* port)
    {
        ServAddrinfo servinfo(port);

        for (Addrinfo* addrinfo : servinfo)
        {
            try
            {
                if (addrinfo != nullptr)
                {
                    SocketInternal socket_internal( addrinfo );

                    socket_internal.set_reuse_addr();
                    socket_internal.bind();
                    socket_internal.listen();

                    return socket_internal;
                }
            }
            catch (...)
            {
                // continue
            }
        }

        throw std::logic_error("Failed to get listening socket!");
    }

    SocketInternal const m_socket_internal;
};
