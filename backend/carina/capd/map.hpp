///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_types.hpp"
#include <capd/map/Map.hpp>

namespace Carina
{

using RMap = capd::map::Map<RMatrix>;
using IMap = capd::map::Map<IMatrix>;

}

namespace capd
{
namespace map
{

extern template class Map<Carina::RMatrix>;
extern template class Map<Carina::IMatrix>;

}
}
