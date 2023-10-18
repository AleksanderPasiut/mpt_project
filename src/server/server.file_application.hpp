///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "server.hpp"

#include <atomic>
#include <filesystem>

struct Params
{
    std::array<unsigned, 5> m_buffer {};
};

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

    void set_default_path(const std::string& default_path);

private:
    std::filesystem::path resource_to_path(const std::string_view& resource) const;

    void send_error(std::stringstream& ss, unsigned code);

    Server m_server;

    std::atomic_flag m_shutdown_request;

    std::filesystem::path m_root;

    std::string m_default_path;

    Params m_params {};
};
