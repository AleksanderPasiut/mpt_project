#include <iostream>
#include "capd/capdlib.h"

#include "carina/timemap_wrapper.hpp"

#include <string>
#include <regex>

std::vector<double> parse_double_vector(const std::string& arg, size_t expected_dimension)
{
    std::vector<double> ret {};
    ret.reserve(expected_dimension);

    std::regex re { R"(([0-9\.\-]+)\;?)" };

    std::smatch res {};

    auto beg = arg.begin();
    auto end = arg.end();

    while (beg != end)
    {
        if (std::regex_search(beg, end, res, re))
        {
            if (res.size() == 2)
            {
                double const value = std::stod( res[1].str() );
                ret.push_back(value);
            }

            beg = res.suffix().first;
        }
        else
        {
            break;
        }
    }

    return ret;
}

std::string compute_timemap(
    std::string func_str,
    std::string initial_condition_values_str,
    double integration_time,
    unsigned order,
    size_t decimal_places)
{
    using namespace Carina;

    IMap func { func_str };

    if (func.dimension() == func.imageDimension())
    {
        const std::vector<double> initial_condition_values = parse_double_vector( initial_condition_values_str, func.dimension() );
        const IVector initial_condition = IVector( initial_condition_values.begin(), initial_condition_values.end() );

        TimemapWrapper<IMap> timemap_wrapper { func, integration_time, order };

        const IVector result = timemap_wrapper(initial_condition);

        std::stringstream ss {};
        ss.precision(decimal_places);
        ss << result;
        return ss.str();
    }
    else
    {
        std::cout << func.dimension() << '\n';
        std::cout << func.imageDimension() << '\n';
        throw std::logic_error("Function dimension and image dimension mismatch!");
    }
}

int main(int argc, char* argv[])
{
    using namespace Carina;

    // example arguments:  "var:x,y;fun:x+y,x-y;" "1.0;2.0;" 1.0 20 20

    if (argc == 6)
    {
        const char* func_str = argv[1];
        const char* initial_condition_values_str = argv[2];
        const double integration_time = std::stod(argv[3]);
        const unsigned order = std::stoul(argv[4]);
        const size_t decimal_places = std::stoul(argv[5]);

        std::cout << compute_timemap(
            func_str,
            initial_condition_values_str,
            integration_time,
            order,
            decimal_places) << '\n';
    }

    return 0;
}
