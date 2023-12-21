///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "addrinfo_iterator.hpp"

#include <stdexcept>

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

    ServAddrinfo(const ServAddrinfo&) = delete;
    ServAddrinfo& operator= (const ServAddrinfo&) = delete;

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
