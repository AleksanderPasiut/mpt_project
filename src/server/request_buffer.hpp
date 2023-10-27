///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <arpa/inet.h>

#include <string_view>
#include <array>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief A buffer to store contents of the HTTP request
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t request_buffer_capacity>
class RequestBuffer
{
public:
    void receive(int fd)
    {
        char* request_buffer_ptr = &( m_request_buffer[0] );
        int bytes_received = recv(fd, request_buffer_ptr, m_request_buffer.size() - 1, 0);

        if (bytes_received < 0)
        {
            throw std::logic_error("Failed to receive data!");
        }

        m_request_buffer_view = std::string_view( request_buffer_ptr, bytes_received );
    }

    const std::string_view& get_view() const noexcept
    {
        return m_request_buffer_view;
    }

private:
    std::array<char, request_buffer_capacity> m_request_buffer {};
    std::string_view m_request_buffer_view { m_request_buffer.data(), 0 };
};
