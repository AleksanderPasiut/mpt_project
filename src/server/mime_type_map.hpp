///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <string_view>

class MimeTypeMap
{
public:
    static MimeTypeMap& get()
    {
        static MimeTypeMap s_instance {};
        return s_instance;
    }

    std::string_view get(const std::string_view& extension)
    {
        auto it = m_container.find(extension);
        if (it != m_container.end())
        {
            return it->second;
        }
        
        return "text/html";
    }

private:
    MimeTypeMap()
    {
        m_container["gif"] = "image/gif";
        m_container["png"] = "image/png";
        m_container["jpg"] = "image/jpeg";
        m_container["css"] = "text/css";
        m_container["txt"] = "text/plain";
        m_container["js"] = "application/javascript";
    }

    std::unordered_map<std::string_view, std::string_view> m_container {};
};
