///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "socket.hpp"

#include <functional>
#include <string>

class OnConnectionNotifier
{
public:
    using Callback = std::function<void(const std::string&)>;

    OnConnectionNotifier()
    {
        m_addr_string.resize(INET6_ADDRSTRLEN, 0);
    }

    OnConnectionNotifier(const OnConnectionNotifier&) = delete;
    OnConnectionNotifier operator= (const OnConnectionNotifier&) = delete;

    void set_callback(Callback callback)
    {
        m_callback = callback;
    }

    void execute(SocketAddressStorage* client_addr);

private:
    std::string m_addr_string {};

    Callback m_callback {};
};
