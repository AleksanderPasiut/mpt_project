///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <stdexcept>

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Unix socket wrapper (specialized for HTTP communication endpoint accessing)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Socket
{
public:
    Socket(const std::string_view& port) : m_socket_wrapper( get_listener_socket(port.data()) )
    {}

    Socket(const Socket&) = delete;
    Socket& operator= (const Socket&) = delete;

    int get_fd() const
    {
        return m_socket_wrapper.get_fd();
    }

private:
    using Addrinfo = addrinfo;

    class AddrinfoIt
    {
    public:
        AddrinfoIt(Addrinfo* ptr) : m_ptr(ptr)
        {}

        AddrinfoIt& operator++ ()
        {
            m_ptr = m_ptr->ai_next;
            return *this;
        }

        Addrinfo* operator* ()
        {
            return m_ptr; 
        }

        bool operator!= (const AddrinfoIt& arg) const noexcept
        {
            return this->m_ptr != arg.m_ptr;
        }

    private:
        Addrinfo* m_ptr;
    };

    class ServAddrinfo
    {
    public:
        ServAddrinfo(const char* port)
        {
            Addrinfo hints {};
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            if (getaddrinfo(nullptr, port, &hints, &m_servinfo) != 0)
            {
                throw std::logic_error("Failed to get addr info!");
            }
        }

        virtual ~ServAddrinfo() noexcept
        {
            if (m_servinfo != nullptr)
            {
                freeaddrinfo(m_servinfo);
            }
        }

        AddrinfoIt begin()
        {
            return AddrinfoIt(m_servinfo);
        }

        AddrinfoIt end()
        {
            return AddrinfoIt(nullptr);
        }
    
    private:
        Addrinfo* m_servinfo {};
    };

    class SocketWrapper
    {
    public:
        SocketWrapper(Addrinfo* addrinfo) : m_addrinfo(addrinfo)
        {
            m_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

            if (m_fd == -1)
            {
                throw std::logic_error("Failed to make a socket!");
            }
        }

        virtual ~SocketWrapper() noexcept
        {
            close(m_fd);
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

    SocketWrapper get_listener_socket(const char *port)
    {
        ServAddrinfo servinfo(port);

        for (Addrinfo* addrinfo : servinfo)
        {
            try
            {
                if (addrinfo != nullptr)
                {
                    SocketWrapper socket_internal { addrinfo };

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

    SocketWrapper const m_socket_wrapper;
};

using SocketAddress = sockaddr;
using SocketAddressStorage = sockaddr_storage;
