///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <string_view>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Notifier that calls specified callback on HTTP request reception
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OnRequestNotifier
{
public:
    using Callback = std::function<void(
        std::stringstream&,
        const std::string_view&,
        const std::string_view&,
        const std::string_view&,
        const std::string_view&)>;

    OnRequestNotifier()
    {}

    OnRequestNotifier(const OnRequestNotifier&) = delete;
    OnRequestNotifier operator= (const OnRequestNotifier&) = delete;

    void set_callback(Callback callback)
    {
        m_callback = callback;
    }

    void execute(std::stringstream& ss, const std::string_view& request_view);

private:
    Callback m_callback {};
};
