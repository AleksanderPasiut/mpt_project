///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include "map.hpp"
#include <capd/dynsys/BasicOdeSolver.hpp>
#include <capd/dynsys/OdeSolver.hpp>

namespace Carina
{

template<typename MapT, bool is_interval>
struct OdeSolverSwitch
{};

template<typename MapT>
struct OdeSolverSwitch<MapT, false>
{
    using Type = capd::dynsys::BasicOdeSolver<MapT>;
};

template<typename MapT>
struct OdeSolverSwitch<MapT, true>
{
    using Type = capd::dynsys::OdeSolver<MapT>;
};

template<typename MapT>
struct OdeSolverInternal
{
    using ScalarType = typename MapT::ScalarType;
    static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
    using Type = typename OdeSolverSwitch<MapT, is_interval>::Type;
};

template<typename MapT>
using OdeSolver = typename OdeSolverInternal<MapT>::Type;

}

namespace capd
{
namespace dynsys
{

extern template class BasicOdeSolver<Carina::RMap>;
extern template class OdeSolver<Carina::IMap>;


}
}
