///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "on_connection_notifier.hpp"

#include <arpa/inet.h>

static void* get_in_addr(SocketAddress* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(reinterpret_cast<sockaddr_in*>(sa)->sin_addr);
    }

    return &(reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr);
}

void OnConnectionNotifier::execute(SocketAddressStorage* client_addr)
{
    if (m_callback)
    {
        void* in_address = get_in_addr(reinterpret_cast<SocketAddress*>(client_addr));

        inet_ntop(
            client_addr->ss_family,
            in_address,
            &( m_addr_string[0] ),
            INET6_ADDRSTRLEN);

        const size_t len = m_addr_string.find_first_of('\0');
        if (len != std::string::npos)
        {
            m_addr_string.resize(len);
        }

        m_callback(m_addr_string);
    }
}
