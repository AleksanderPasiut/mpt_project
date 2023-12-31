///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "on_request_notifier.hpp"

#include <iostream>
#include <regex>

using It = std::string_view::const_iterator;

struct PrimaryData
{
    bool valid { false };
    std::string_view method {};
    std::string_view uri {};
    std::string_view protocol {};
    It end {};
};

struct CookieData
{
    bool valid { false };
    std::string_view cookie {};
    It end {};
};

static PrimaryData extract_primary_data(const std::string_view& request_view)
{
    std::regex re( R"(([^\s]*)\s([^\s]*)\s([^\n\r]*))");

    std::match_results<It> results {};
    if (std::regex_search(request_view.begin(), request_view.end(), results, re))
    {
        if (results.size() == 4)
        {
            return PrimaryData
            {
                .valid = true,
                .method = std::string_view( results[1].first, results[1].length() ),
                .uri = std::string_view( results[2].first, results[2].length() ),
                .protocol = std::string_view( results[3].first, results[3].length() ),
                .end = results[3].second
            };
        }
        else
        {
            std::cerr << __func__ << " unexpected regex results size! " << results.size() << "\n";
        }
    }
    else
    {
        std::cerr << __func__ << " regex search failed!\n";
    }

    return PrimaryData();
}

static CookieData extract_cookie_data(It begin, It end)
{
    std::regex re( R"(Cookie: ([^\n\r]*))" );

    std::match_results<It> results {};
    if (std::regex_search(begin, end, results, re))
    {
        if (results.size() == 2)
        {
            return CookieData
            {
                .valid = true,
                .cookie = std::string_view( results[1].first, results[1].length() ),
                .end = results[1].second
            };
        }
        else
        {
            std::cerr << __func__ << " unexpected regex results size! " << results.size() << "\n";
        }
    }
    else
    {
        std::cerr << __func__ << " regex search failed!\n";
    }

    return CookieData();
}

static std::string_view extract_contents(It begin, It end)
{
    std::regex re( R"((\r\n\r\n|\r\r|\n\n)(.*))" );

    std::match_results<It> results {};
    if (std::regex_search(begin, end, results, re))
    {
        if (results.size() == 3)
        {
            return std::string_view( results[2].first, results[2].length() );
        }
        else
        {
            std::cerr << __func__ << " unexpected regex results size! " << results.size() << "\n";
        }
    }
    else
    {
        std::cerr << __func__ << " regex search failed!\n";
    }

    return std::string_view();
}

void OnRequestNotifier::execute(std::stringstream& ss, const std::string_view& request_view)
{
    if (m_callback)
    {
        const PrimaryData primary_data = extract_primary_data(request_view);

        if (primary_data.valid)
        {
            if (primary_data.protocol == "HTTP/1.1")
            {
                It end = primary_data.end;

                const CookieData cookie_data = extract_cookie_data(end, request_view.end());

                if (cookie_data.valid)
                {
                    end = cookie_data.end;
                }

                const std::string_view contents = (primary_data.method == "POST") ? extract_contents(primary_data.end, request_view.end()) : std::string_view();
                m_callback( std::ref(ss), std::cref(primary_data.method), std::cref(primary_data.uri), std::cref(contents), std::cref(cookie_data.cookie) );
            }
        }
    }
}
