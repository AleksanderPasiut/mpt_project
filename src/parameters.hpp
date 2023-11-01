///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "server/custom_response.hpp"

#include <string_view>
#include <array>

class Params
{
public:
    CustomResponse handle_parameter_set(const std::string_view& query);
    CustomResponse handle_parameters_get(const std::string_view&);
    CustomResponse set_string_parameter(const std::string_view&);
    CustomResponse get_string_parameter(const std::string_view&);

private:
    std::array<unsigned, 5> m_buffer {};

    std::string m_string_parameter {};
};
