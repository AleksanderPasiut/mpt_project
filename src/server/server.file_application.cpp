///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "server.file_application.hpp"

ServerFileApplication::ServerFileApplication(const std::string_view& port, const std::filesystem::path& root)
    : m_server(port)
    , m_impl(root)
{
    m_shutdown_request.test_and_set();

    m_server.set_on_request_callback(
        [this](std::stringstream& ss, const std::string_view& method, const std::string_view& uri, const std::string_view& contents ) -> void
    {
        m_impl.handle_request(ss, method, uri, contents);
    });
}

int ServerFileApplication::run()
{
    while (m_server.update())
    {
        if (m_shutdown_request.test_and_set() == false)
        {
            return 1;
        }
    }
    
    return 0;
}

void ServerFileApplication::request_shutdown()
{
    m_shutdown_request.clear();
}
