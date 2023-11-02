///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "process.hpp"

#include <sstream>

class CapdProcess
{
public:
    struct Params
    {
        const std::string executable_path;
        const std::string func_str;
        const std::string initial_condition_values_str;
        double integration_time;
        unsigned order;
        size_t decimal_places;
    };

    CapdProcess(const Params& params) : m_process(gen_cmd(params))
    {}

    CapdProcess(const CapdProcess&) = delete;
    CapdProcess& operator= (const CapdProcess&) = delete;

    std::string get_resp()
    {
        return m_process.readsome(1024);
    }

private:
    static std::string gen_cmd(const Params& params)
    {
        std::stringstream ss {};
        ss << params.executable_path << " ";
        ss << "\"" << params.func_str << "\" ";
        ss << "\"" << params.initial_condition_values_str << "\" ";
        ss << params.integration_time << ' ';
        ss << params.order << ' ';
        ss << params.decimal_places;

        const std::string ret = ss.str();
        return ret;
    }

    Process m_process;
};
