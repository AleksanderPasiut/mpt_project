///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "qr_code.hpp"
#include "stream_wrapper.hpp"

#include <qr_gen.hpp>

#include <iostream>
#include <fstream>
#include <regex>

CustomResponse get_qr_code(const std::string_view& query, const std::string_view& cookie)
{
    std::cout << "get_qr_code " << cookie << '\n';

    std::ifstream fs("qr_code.bmp", std::ifstream::binary);
    if (fs)
    {
        std::string buffer {};
        buffer.resize(65536);
        size_t n = fs.readsome(&buffer[0], buffer.size());
        buffer.resize(n);
        return CustomResponse(200, "image/bmp", buffer);
    }

    return CustomResponse(404);
}

void generate_qr_code_bmp(const std::string& port)
{
    StreamWrapper hostname_cmd("hostname -I");

    std::string hostname_cmd_resp {};
    hostname_cmd.read(hostname_cmd_resp, 1024);

    std::regex re { R"(([0-9\.]*))" };
    std::smatch results {};
    if (std::regex_search(hostname_cmd_resp, results, re))
    {
        if (results.size() == 2)
        {
            const std::string hostname = results[1];
            const std::string url = "http://" + hostname + ":" + port;
            run_qr_gen(url.c_str(), "qr_code.bmp");
        }
        else
        {
            std::cerr << __func__ << " unexpected regex results size! " << results.size() << "\n";
        }
    }
    else
    {
        std::cerr << __func__ << " regex search failed!\n";
    }

}
