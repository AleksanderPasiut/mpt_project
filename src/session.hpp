///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "server/custom_response.hpp"
#include "capd_process.hpp"

#include <string_view>
#include <array>
#include <iostream>
#include <memory>

class Session
{
public:
    Session(std::string id) : m_id(id)
    {}

    Session(const Session&) = delete;
    Session& operator= (const Session&) = delete;

    Session(Session&&) = default;

    CustomResponse handle_parameter_set(const std::string_view& query);
    CustomResponse handle_parameters_get(const std::string_view& query);

    template<unsigned idx>
    CustomResponse set_string(const std::string_view& payload)
    {
        std::cout << __func__ <<  '\n';

        m_string_parameter[idx] = payload;
        return CustomResponse(200);
    }

    CustomResponse get_string_output(const std::string_view&);

    CustomResponse compute(const std::string_view&);

private:
    void kill_capd_process();

    const std::string m_id {};
    
    std::array<unsigned, 2> m_buffer { 20, 15 };

    std::array<std::string, 3> m_string_parameter {};
    std::string m_string_output {};

    std::unique_ptr<CapdProcess> m_capd_process_ptr {};
};
