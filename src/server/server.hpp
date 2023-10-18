///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "socket.hpp"
#include "on_connection_notifier.hpp"
#include "on_request_notifier.hpp"
#include "request_buffer.hpp"

class Server
{
public:
    using OnConnectionCallback = OnConnectionNotifier::Callback;
    using OnRequestCallback = OnRequestNotifier::Callback;

    Server(const std::string_view& port);

    Server(const Server&) = delete;
    Server& operator= (const Server&) = delete;

    bool update();

    void set_on_connection_callback(OnConnectionCallback callback)
    {
        m_on_connection_notifier.set_callback(callback);
    }

    void set_on_request_callback(OnRequestCallback callback)
    {
        m_on_request_notifier.set_callback(callback);
    }

private:
    Socket m_socket;

    OnConnectionNotifier m_on_connection_notifier;
    OnRequestNotifier m_on_request_notifier;

    RequestBuffer m_request_buffer;
};

