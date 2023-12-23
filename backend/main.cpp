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
        std::stringstream ss {};
        ss << "Function dimension and image dimension mismatch: ";
        ss << func.dimension() << ' ' << func.imageDimension() << '\n';
        throw std::logic_error(ss.str());
    }
}

const char * validate_not_empty(const char* ptr, const char* exception_msg)
{
    if (!ptr || !*ptr)
    {
        throw std::logic_error(exception_msg);
    }
    return ptr;
}

const char * validate_formula_string(const char * ptr)
{
    return validate_not_empty( ptr, "Empty formula string provided!" );
}

const char * validate_initial_conditions_string(const char * ptr)
{
    return validate_not_empty( ptr, "Empty initial condition values!" );
}

double parse_integration_time(const char * ptr)
{
    try
    {
        return std::stod(ptr);
    }
    catch (...)
    {
        throw std::logic_error("Failed to parse integration time value!");
    }
}

unsigned parse_order(const char* ptr)
{
    try
    {
        return std::stoul(ptr);
    }
    catch (...)
    {
        throw std::logic_error("Failed to parse order value!");
    }
}

size_t parse_decimal_places(const char* ptr)
{
    try
    {
        return std::stoul(ptr);
    }
    catch (...)
    {
        throw std::logic_error("Failed to parse decimal places value!");
    }
}

int main(int argc, char* argv[])
{
    using namespace Carina;

    // example arguments:  "var:x,y;fun:x+y,x-y;" "1.0;2.0;" 1.0 20 20
    try
    {
        if (argc == 7)
        {
            const char* func_str
            {
                validate_formula_string( argv[2] )
            };

            const char* initial_condition_values_str
            {
                validate_initial_conditions_string( argv[3] )
            };

            const double integration_time
            {
                parse_integration_time( argv[4] )
            };

            const unsigned order
            {
                parse_order( argv[5] )
            };

            const size_t decimal_places
            {
                parse_decimal_places( argv[6] )
            };
            
            const std::string output = compute_timemap(
                func_str,
                initial_condition_values_str,
                integration_time,
                order,
                decimal_places);

            std::cout << output << '\n';
        }
        else
        {
            throw std::logic_error("Internal error");
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}
