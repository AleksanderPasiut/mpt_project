///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "server.hpp"
#include "server.file_application.impl.hpp"

#include <atomic>
#include <filesystem>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Application layer implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ServerFileApplication
{
public:
    ServerFileApplication(const std::string_view& port, const std::filesystem::path& root);

    ServerFileApplication(const ServerFileApplication&) = delete;
    ServerFileApplication& operator= (const ServerFileApplication&) = delete;

    virtual ~ServerFileApplication() noexcept
    {}

    int run();

    void request_shutdown();

    ServerFileApplicationImpl& get_impl() noexcept
    {
        return m_impl;
    }

private:
    Server m_server;

    ServerFileApplicationImpl m_impl;

    std::atomic_flag m_shutdown_request {};
};
