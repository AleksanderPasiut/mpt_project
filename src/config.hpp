///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stddef.h>
#include <string>

class Config
{
public:
    static Config& get()
    {
        static Config g_instance {};
        return g_instance;
    }

    std::string get_backend_process_path() const
    {
        return "backend/capd_backend";
    }

    size_t get_max_capd_process_output_length() const
    {
        return 1024;
    }

    int get_capd_process_timeout_ms() const
    {
        return 5000;
    }

private:
    Config() = default;

    Config(const Config&) = delete;
    Config& operator= (const Config&) = delete;
};
