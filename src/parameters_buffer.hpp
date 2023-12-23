///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "server/custom_response.hpp"

#include <string_view>
#include <array>
#include <iostream>

class ParametersBuffer
{
public:
    CustomResponse handle_parameter_set(const std::string_view& query, const std::string_view& cookie);
    CustomResponse handle_parameters_get(const std::string_view& query, const std::string_view& cookie);

    template<unsigned idx>
    CustomResponse set_string(const std::string_view& payload, const std::string_view& cookie)
    {
        std::cout << __func__ <<  '\n';

        m_string_parameter[idx] = payload;
        return CustomResponse(200);
    }

    CustomResponse get_string_output(const std::string_view&, const std::string_view& cookie);

    CustomResponse compute(const std::string_view&, const std::string_view& cookie);

private:
    std::array<unsigned, 2> m_buffer { 20, 15 };

    std::array<std::string, 3> m_string_parameter {};
    std::string m_string_output {};
};
