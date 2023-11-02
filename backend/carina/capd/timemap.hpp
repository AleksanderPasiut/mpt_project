///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include "ode_solver.hpp"
#include <capd/poincare/TimeMap.hpp>

namespace Carina
{

template<typename MapT>
using Timemap = capd::poincare::TimeMap<OdeSolver<MapT>>;

}

namespace capd
{
namespace poincare
{

extern template class TimeMap<Carina::OdeSolver<Carina::RMap>>;
extern template class TimeMap<Carina::OdeSolver<Carina::IMap>>;


}
}
