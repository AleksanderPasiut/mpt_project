///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "server.file_application.impl.hpp"
#include "mime_type_map.hpp"
#include "string_view.tool.hpp"
#include "uri.hpp"
#include "custom_response.hpp"

#include <fstream>
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
        if (resp.get_cookie().size() > 0)
        {
            ss << "Set-Cookie: " << resp.get_cookie() << '\n';
        }
        ss << "Content-Type: " << resp.get_content_type() << "\n\n";
        ss << resp.get_content();
    }
    else
    {
        ss << '\n';
    }
}


void ServerFileApplicationImpl::fill_file_response(std::stringstream& ss, const std::string_view& uri)
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


void ServerFileApplicationImpl::handle_request(
    std::stringstream& ss,
    const std::string_view& method,
    const std::string_view& uri,
    const std::string_view& contents,
    const std::string_view& cookie )
{
    const Uri get_uri(uri);

    if (method == "GET")
    {
        auto it = m_custom_callbacks.find(std::string(get_uri.get_path()));
        if (it != m_custom_callbacks.end())
        {
            fill_custom_response(ss, (it->second)(get_uri.get_query(), cookie));
        }
        else
        {
            fill_file_response(ss, uri);
        }
    }

    if (method == "POST")
    {
        auto it = m_on_post_callbacks.find(std::string(get_uri.get_path()));
        if (it != m_on_post_callbacks.end())
        {
            fill_custom_response(ss, (it->second)(contents, cookie));
        }
    }
}



void ServerFileApplicationImpl::set_default_path(const std::string& default_path)
{
    m_default_path = default_path;
}

void ServerFileApplicationImpl::reset_custom_handlers()
{
    m_custom_callbacks.clear();
}
    
void ServerFileApplicationImpl::register_custom_handler(std::string path, CustomCallback callback)
{
    m_custom_callbacks[path] = callback;
}

void ServerFileApplicationImpl::reset_on_post_handlers()
{
    m_on_post_callbacks.clear();
}

void ServerFileApplicationImpl::register_on_post_handler(std::string path, CustomCallback callback)
{
    m_on_post_callbacks[path] = callback;
}

std::filesystem::path ServerFileApplicationImpl::convert_uri_path_to_local_path(const std::string_view& uri) const
{
    if (uri == "/" && m_default_path != "")
    {
        return m_root / m_default_path;
    }

    return m_root / std::filesystem::path(uri.substr(1, uri.size() - 1));
}
