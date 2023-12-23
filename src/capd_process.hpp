///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "process.hpp"

#include "capd_process.params.hpp"

class CapdProcess
{
public:
    CapdProcess(const CapdProcessParams& params) : m_process(params.get_cmd(), 1024)
    {}

    CapdProcess(const CapdProcess&) = delete;
    CapdProcess& operator= (const CapdProcess&) = delete;

    State get_state() const noexcept
    {
        return m_process.get_state();
    }

    std::string get_and_clear_output()
    {
        return m_process.get_and_clear_output();
    }

private:
    Process m_process;
};
