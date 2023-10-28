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

    void set_default_path(const std::string& default_path);

private:
    std::filesystem::path convert_uri_to_path(const std::string_view& uri) const;

    void send_error(std::stringstream& ss, unsigned code);

    bool handle_parameter_set(std::stringstream& ss, const std::string_view& query);

    bool handle_parameters_get(std::stringstream& ss, const std::string_view& path);

    Server m_server;

    std::atomic_flag m_shutdown_request;

    std::filesystem::path m_root;

    std::string m_default_path;

    Params m_params {};
};
