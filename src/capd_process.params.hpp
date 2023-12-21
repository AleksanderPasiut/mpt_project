///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <sstream>

class CapdProcessParams
{
public:
    CapdProcessParams(
        const std::string& func_str,
        const std::string& initial_condition_values_str,
        const std::string& integration_time_str,
        unsigned order,
        unsigned decimal_places)
    {
        std::stringstream ss {};
        ss << "backend/capd_backend ";
        ss << process_str_param(func_str, "formula string")  << ' ';
        ss << process_str_param(initial_condition_values_str, "initial condition values string")  << ' ';
        ss << process_str_param(integration_time_str, "integration time string")  << ' ';
        ss << order << ' ';
        ss << decimal_places;

        m_cmd = ss.str();
    }

    const std::string& get_cmd() const noexcept
    {
        return m_cmd;
    }

private:
    static std::string process_str_param(
        const std::string& arg,
        const std::string& param_name)
    {
        if( arg.find_first_of('\"') != std::string::npos)
        {
            std::stringstream ss {};
            ss << "Invalid characters in " << param_name;
            throw std::logic_error(ss.str());
        }

        return '\"' + arg + '\"';
    }

    std::string m_cmd;
};
