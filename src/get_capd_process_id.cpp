///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stream_wrapper.hpp"
#include "config.hpp"

#include <iostream>
#include <regex>

static std::string get_capd_pid_command()
{
    std::stringstream ss {};
    ss << "ps a | grep ";
    ss << Config::get().get_backend_process_path();
    return ss.str();
}

pid_t get_capd_pid(const std::string_view& session_id)
{
    StreamWrapper ps_pid_getter( get_capd_pid_command() );

    std::string ps_pid_resp {};
    ps_pid_getter.read(ps_pid_resp, 65536);

    std::stringstream ss {};
    ss.str(ps_pid_resp);

    while (true)
    {
        std::string line {};
        std::getline(ss, line, '\n');

        if (ss.eof())
        {
            break;
        }

        std::regex re { R"(^\s*([^\s]*)\s*[^\s]*\s*[^\s]*\s*[^\s]*\s*([^\s]*)\s*([^\s]*).*$)" };
        std::smatch results {};
        if (std::regex_search(line, results, re))
        {
            if (results.size() == 4)
            {
                const std::string session_id_str = results[3].str();

                if (session_id_str == session_id)
                {
                    const std::string app_str = results[2].str();

                    if (app_str == Config::get().get_backend_process_path())
                    {
                        const std::string pid_str = results[1].str();
                        pid_t ret = std::stoi(pid_str);
                        return ret;
                    }
                }
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
    }

    return -1;
}

