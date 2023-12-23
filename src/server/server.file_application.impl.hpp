///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "custom_response.hpp"

#include <filesystem>
#include <functional>

class ServerFileApplicationImpl
{
public:
    using CustomCallback = std::function<CustomResponse(std::string_view, std::string_view)>;

    ServerFileApplicationImpl(const std::filesystem::path& root) : m_root(root)
    {}

    ServerFileApplicationImpl(const ServerFileApplicationImpl&) = delete;
    ServerFileApplicationImpl& operator= (const ServerFileApplicationImpl&) = delete;

    virtual ~ServerFileApplicationImpl() noexcept
    {}

    void handle_request(
        std::stringstream& ss,
        const std::string_view& method,
        const std::string_view& uri,
        const std::string_view& contents,
        const std::string_view& cookie );

    void set_default_path(const std::string& default_path);

    void reset_custom_handlers();
    
    void register_custom_handler(std::string path, CustomCallback callback);

    void reset_on_post_handlers();

    void register_on_post_handler(std::string path, CustomCallback callback);

private:
    std::filesystem::path convert_uri_path_to_local_path(const std::string_view& uri) const;

    void fill_file_response(std::stringstream& ss, const std::string_view& uri);

    std::filesystem::path m_root;

    std::string m_default_path {};

    std::unordered_map<std::string, CustomCallback> m_custom_callbacks {};

    std::unordered_map<std::string, CustomCallback> m_on_post_callbacks {};
};
