///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <string>

inline const std::string_view get_string_view_from_string(const std::string& arg)
{
    return std::string_view( arg.data(), arg.size() );
}
