///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "server.file_application.hpp"
#include "mime_type_map.hpp"
#include "string_view.tool.hpp"
#include "uri.hpp"
#include "custom_response.hpp"

#include <fstream>
#include <regex>
#include <iostream>




static std::string_view get_extension(const std::string& arg)
{
    const std::size_t dot_idx = arg.find_last_of('.');

    if (dot_idx != std::string::npos && dot_idx < arg.size())
    {
        const std::string_view arg_view = get_string_view_from_string(arg);
        return arg_view.substr(dot_idx + 1, arg_view.size() - dot_idx);
    }

    return std::string_view();
}

static std::string_view get_content_type(const std::string& arg)
{
    const std::string_view extension = get_extension(arg);
    return MimeTypeMap::get().get(extension);
}

static void fill_http_status_code(std::stringstream& ss, unsigned code)
{
    ss << "HTTP/1.1 " << code << ' ';

    switch (code)
    {
        case 200: ss << "OK"; break;
        case 403: ss << "FORBIDDEN"; break;
        case 404: ss << "NOT FOUND"; break;
        case 501: ss << "NOT IMPLEMENTED "; break;
        default: ss << "INTERNAL SERVER ERROR"; break;
    }

    ss << '\n';
}

static void fill_custom_response(std::stringstream& ss, const CustomResponse& resp)
{
    fill_http_status_code(ss, resp.get_code());

    if (resp.get_content_type().size() > 0)
    {
        ss << "Content-Type: " << resp.get_content_type() << "\n\n";
        ss << resp.get_content();
    }
    else
    {
        ss << '\n';
    }
}





void Internal::handle_request(std::stringstream& ss, const std::string_view& method, const std::string_view& uri, const std::string_view& contents )
{
    const Uri get_uri(uri);

    if (method == "GET")
    {
        auto it = m_custom_callbacks.find(std::string(get_uri.get_path()));
        if (it != m_custom_callbacks.end())
        {
            fill_custom_response(ss, (it->second)(get_uri.get_query()));
        }
        else
        {
            try
            {
                const std::filesystem::path path = convert_uri_path_to_local_path(uri);

                std::ifstream fs(path);

                if (fs)
                {
                    std::string_view content_type = get_content_type(path.string());
                    fill_http_status_code(ss, 200);
                    ss << "Content-Type: " << content_type;
                    ss << "\n\n";
                    ss << fs.rdbuf();
                    fs.close();
                }
                else
                {
                    fill_http_status_code(ss, 404);
                }
            }
            catch (std::exception& e)
            {
                fill_http_status_code(ss, 500);
            }
        }
    }

    if (method == "POST")
    {
        auto it = m_on_post_callbacks.find(std::string(get_uri.get_path()));
        if (it != m_on_post_callbacks.end())
        {
            fill_custom_response(ss, (it->second)(contents));
        }
    }
}

void Internal::request_shutdown()
{
    m_shutdown_request.clear();
}

void Internal::set_default_path(const std::string& default_path)
{
    m_default_path = default_path;
}

void Internal::reset_custom_handlers()
{
    m_custom_callbacks.clear();
}
    
void Internal::register_custom_handler(std::string path, CustomCallback callback)
{
    m_custom_callbacks[path] = callback;
}

void Internal::reset_on_post_handlers()
{
    m_on_post_callbacks.clear();
}

void Internal::register_on_post_handler(std::string path, CustomCallback callback)
{
    m_on_post_callbacks[path] = callback;
}

std::filesystem::path Internal::convert_uri_path_to_local_path(const std::string_view& uri) const
{
    if (uri == "/" && m_default_path != "")
    {
        return m_root / m_default_path;
    }

    return m_root / std::filesystem::path(uri.substr(1, uri.size() - 1));
}




ServerFileApplication::ServerFileApplication(const std::string_view& port, const std::filesystem::path& root)
    : m_server(port)
    , m_internal(root)
{
    m_server.set_on_request_callback(
        [this](std::stringstream& ss, const std::string_view& method, const std::string_view& uri, const std::string_view& contents ) -> void
    {
        m_internal.handle_request(ss, method, uri, contents);
    });
}

int ServerFileApplication::run()
{
    while (m_server.update())
    {
        if (m_internal.update() == false)
        {
            return 1;
        }
    }
    
    return 0;
}
