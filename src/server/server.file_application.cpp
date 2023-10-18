///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "server.file_application.hpp"

#include <fstream>
#include <regex>
#include <iostream>

static std::string_view get_content_type(const std::string& arg)
{
    const std::size_t dot_idx = arg.find_last_of('.');

    if (dot_idx != std::string::npos && dot_idx < arg.size())
    {
        const std::string extension = arg.substr(dot_idx + 1, arg.size() - dot_idx);

        if (extension == "gif")
        {
            return "image/gif";
        }

        if (extension == "png")
        {
            return "image/png";
        }

        if (extension == "jpg")
        {
            return "image/jpeg";
        }
    }

    return "text/html";
}

ServerFileApplication::ServerFileApplication(const std::string_view& port, const std::filesystem::path& root)
    : m_server(port)
    , m_shutdown_request()
    , m_root(root)
    , m_default_path()
{
    m_shutdown_request.test_and_set();

    m_server.set_on_request_callback(
        [this](std::stringstream& ss, const std::string_view& method, const std::string_view& resource) -> void
    {
        if (method == "GET")
        {
            if (handle_parameter_set(ss, resource) == false)
            {
                if (handle_parameters_get(ss, resource) == false)
                {
                    try
                    {
                        const std::filesystem::path path = resource_to_path(resource);

                        std::ifstream fs(path);

                        if (fs)
                        {
                            std::string_view content_type = get_content_type(path.string());
                            ss << "HTTP/1.1 200 OK\n";
                            ss << "Content-Type: " << content_type;
                            ss << "\n\n";
                            ss << fs.rdbuf();
                            fs.close();
                        }
                        else
                        {
                            send_error(ss, 404);
                        }
                    }
                    catch (std::exception& e)
                    {
                        send_error(ss, 500);
                    }
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

std::filesystem::path ServerFileApplication::resource_to_path(const std::string_view& resource) const
{
    if (resource == "/" && m_default_path != "")
    {
        return m_root / m_default_path;
    }

    return m_root / std::filesystem::path(resource.substr(1, resource.size() - 1));
}

void ServerFileApplication::send_error(std::stringstream& ss, unsigned code)
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

    ss << "\n\n";
}

bool ServerFileApplication::handle_parameter_set(std::stringstream& ss, const std::string_view& resource)
{
    std::regex re("/value\\?input([0-9]*)=([0-9\\.]*)");
    std::match_results<std::string_view::iterator> res {};
    if ( std::regex_match(resource.begin(), resource.end(), res, re) )
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

bool ServerFileApplication::handle_parameters_get(std::stringstream& ss, const std::string_view& resource)
{
    if (resource == "/values.txt")
    {
        ss << "HTTP/1.1 200 OK\n";
        ss << "Content-Type: text/plain\n\n";
        
        auto it = m_params.m_buffer.begin();
        ss << *it;
        for (++it; it != m_params.m_buffer.end(); ++it)
        {
            ss << ";" << *it;
        }

        return true;
    }

    return false;
}
