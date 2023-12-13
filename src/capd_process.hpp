///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "process.hpp"

#include "capd_process.params.hpp"

class CapdProcess
{
public:
    CapdProcess(const CapdProcessParams& params) : m_process(params.get_cmd())
    {}

    CapdProcess(const CapdProcess&) = delete;
    CapdProcess& operator= (const CapdProcess&) = delete;

    std::string get_resp()
    {
        return m_process.readsome(1024);
    }

private:
    Process m_process;
};
