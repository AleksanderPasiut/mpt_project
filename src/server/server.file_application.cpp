///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "server.file_application.hpp"
#include "mime_type_map.hpp"
#include "string_view.tool.hpp"
#include "uri.hpp"

#include <fstream>
#include <regex>
#include <iostream>

class CustomResponse
{
public:
    CustomResponse(unsigned code) : m_code(code)
    {}

    CustomResponse(unsigned code, std::string_view content_type, std::string content)
        : m_code(code)
        , m_content_type(content_type)
        , m_content(content)
    {}

    unsigned get_code() const
    {
        return m_code;
    }

    const std::string_view& get_content_type() const
    {
        return m_content_type;
    }

    const std::string& get_content() const
    {
        return m_content;
    }

private:
    unsigned m_code;
    std::string_view m_content_type;
    std::string m_content;

};

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

static void fill_custom_response(std::stringstream& ss, CustomResponse& resp)
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

ServerFileApplication::ServerFileApplication(const std::string_view& port, const std::filesystem::path& root)
    : m_server(port)
    , m_shutdown_request()
    , m_root(root)
    , m_default_path()
{
    m_shutdown_request.test_and_set();

    m_server.set_on_request_callback(
        [this](std::stringstream& ss, const std::string_view& method, const std::string_view& uri) -> void
    {
        if (method == "GET")
        {
            const Uri get_uri(uri);

            if (get_uri.get_path() == "/value")
            {
                if (handle_parameter_set(ss, get_uri.get_query()) == false)
                {
                    fill_http_status_code(ss, 500);
                }
            }
            else if (get_uri.get_path() == "/values.txt")
            {
                handle_parameters_get(ss, get_uri.get_path());
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
    });
}

int ServerFileApplication::run()
{
    while (m_server.update())
    {
        if (m_shutdown_request.test_and_set() == false)
        {
            return 1;
        }
    }
    
    return 0;
}

void ServerFileApplication::request_shutdown()
{
    m_shutdown_request.clear();
}

void ServerFileApplication::set_default_path(const std::string& default_path)
{
    m_default_path = default_path;
}

std::filesystem::path ServerFileApplication::convert_uri_path_to_local_path(const std::string_view& uri) const
{
    if (uri == "/" && m_default_path != "")
    {
        return m_root / m_default_path;
    }

    return m_root / std::filesystem::path(uri.substr(1, uri.size() - 1));
}

void ServerFileApplication::send_error(std::stringstream& ss, unsigned code)
{
    fill_http_status_code(ss, code);
    ss << '\n';
}

bool ServerFileApplication::handle_parameter_set(std::stringstream& ss, const std::string_view& query)
{
    std::regex re( R"(^input([0-9]*)=([0-9\.]*)$)");
    std::match_results<std::string_view::iterator> res {};
    if ( std::regex_match(query.begin(), query.end(), res, re) )
    {
        if (res.size() == 3)
        {
            size_t idx = std::stoi( res[1].str() );
            double value = std::stod( res[2].str() );

            if (idx < m_params.m_buffer.size())
            {
                m_params.m_buffer[idx] = value;
            }

            send_error(ss, 200);
        }
        else
        {
            send_error(ss, 500);
        }

        return true;
    }

    return false;
}

void ServerFileApplication::handle_parameters_get(std::stringstream& ss, const std::string_view& path)
{
    std::stringstream sst {};

    auto it = m_params.m_buffer.begin();
    sst << *it;
    for (++it; it != m_params.m_buffer.end(); ++it)
    {
        sst << ";" << *it;
    }

    CustomResponse resp(200, "text/plain", sst.str());

    fill_custom_response(ss, resp);
    // fill_http_status_code(ss, 200);
    // ss << "Content-Type: text/plain\n\n";
    
    
}
