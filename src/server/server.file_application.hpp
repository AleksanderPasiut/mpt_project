///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "server.hpp"
#include "custom_response.hpp"

#include <atomic>
#include <filesystem>
#include <functional>

class Internal
{
public:
    using CustomCallback = std::function<CustomResponse(std::string_view)>;

    Internal(const std::filesystem::path& root) : m_root(root)
    {
        m_shutdown_request.test_and_set();
    }

    Internal(const Internal&) = delete;
    Internal& operator= (const Internal&) = delete;

    virtual ~Internal() noexcept
    {}

    bool update()
    {
        return m_shutdown_request.test_and_set();
    }

    void handle_request(std::stringstream& ss, const std::string_view& method, const std::string_view& uri, const std::string_view& contents );

    void request_shutdown();

    void set_default_path(const std::string& default_path);

    void reset_custom_handlers();
    
    void register_custom_handler(std::string path, CustomCallback callback);

    void reset_on_post_handlers();

    void register_on_post_handler(std::string path, CustomCallback callback);

private:
    std::filesystem::path convert_uri_path_to_local_path(const std::string_view& uri) const;


    std::atomic_flag m_shutdown_request {};

    std::filesystem::path m_root;

    std::string m_default_path {};

    std::unordered_map<std::string, CustomCallback> m_custom_callbacks {};

    std::unordered_map<std::string, CustomCallback> m_on_post_callbacks {};
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

    Internal& get_internal() noexcept
    {
        return m_internal;
    }

private:

    // void send_error(std::stringstream& ss, unsigned code);

    bool handle_parameter_set(std::stringstream& ss, const std::string_view& query);

    void handle_parameters_get(std::stringstream& ss, const std::string_view& path);

    Server m_server;

    Internal m_internal;
};
