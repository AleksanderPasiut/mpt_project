///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "server.hpp"

#include <sstream>
#include <string.h>
#include <arpa/inet.h>

Server::Server(const std::string_view& port) : m_socket(port)
{}

static void send_sstream_contents(int fd, std::stringstream& ss)
{
    static const size_t SEND_BUFFER_SIZE = 16;
    char send_buffer[SEND_BUFFER_SIZE];
    for(;;)
    {
        const size_t stored_bytes = ss.readsome(send_buffer, SEND_BUFFER_SIZE);
        if (stored_bytes > 0)
        {
            const ssize_t sent_bytes = send(fd, send_buffer, stored_bytes, 0);
            if (stored_bytes != sent_bytes)
            {
                throw std::logic_error("Failed to send data!");
            }
        }
        else
        {
            break;
        }
    }
}

bool Server::update()
{
    SocketAddressStorage client_addr;
    socklen_t s_in_size = sizeof(client_addr);

    const int client_fd = accept(m_socket.get_fd(), (sockaddr *)&client_addr, &s_in_size);
    if (client_fd != -1)
    {
        m_on_connection_notifier.execute( &client_addr );

        m_request_buffer.receive(client_fd);

        std::stringstream ss {};
        m_on_request_notifier.execute( ss, m_request_buffer.get_view() );
        send_sstream_contents(client_fd, ss);

        close(client_fd);
    }
    else
    {
        std::stringstream ss;
        ss << "accept error (" << strerror(errno) << ")\n";
        throw std::logic_error(ss.str());
    }

    return true;
}
