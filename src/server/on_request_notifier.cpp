///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "on_request_notifier.hpp"

#include <regex>

void OnRequestNotifier::execute(std::stringstream& ss, const std::string_view& request_view)
{
    if (m_callback)
    {
        std::regex re("([^\\s]*)\\s([^\\s]*)\\s([^\\n\\r]*)");

        using It = std::string_view::const_iterator;

        std::match_results<It> results {};
        std::regex_search(request_view.begin(), request_view.end(), results, re);

        if (results.size() == 4)
        {
            const std::string_view method( results[1].first, results[1].length() );
            const std::string_view resource( results[2].first, results[2].length() );
            const std::string_view protocol( results[3].first, results[3].length() );

            if (protocol == "HTTP/1.1")
            {
                m_callback( std::ref(ss), std::cref(method), std::cref(resource) );
            }
        }
    }
}
