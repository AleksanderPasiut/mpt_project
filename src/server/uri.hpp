///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "string_view.tool.hpp"

#include <regex>

class Uri
{
public:
    Uri(const std::string_view& uri)
    {
        std::regex re { R"(^([^\?]*)(|\?([^\?]*))$)" };

        std::match_results<std::string_view::iterator> res {};
        if ( std::regex_match(uri.begin(), uri.end(), res, re) )
        {
            if (res.size() == 3) // query not present
            {
                m_path = res[1].str();
                m_query = "";
            }

            if (res.size() == 4) // query present
            {
                m_path = res[1].str();
                m_query = res[3].str();
            }
        }
    };

    std::string_view get_path() const
    {
        return get_string_view_from_string(m_path);
    }

    std::string_view get_query() const
    {
        return get_string_view_from_string(m_query);
    }

private:
    std::string m_path;
    std::string m_query;
};
