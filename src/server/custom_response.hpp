///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <string>

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
