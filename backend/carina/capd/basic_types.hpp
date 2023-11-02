///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd/capdlib.h>

namespace Carina
{

using Real = double;
using Interval = capd::intervals::Interval<Real, capd::rounding::DoubleRounding>;

using RVector = capd::vectalg::Vector<Real, 0>;
using IVector = capd::vectalg::Vector<Interval, 0>;

using RMatrix = capd::vectalg::Matrix<Real, 0, 0>;
using IMatrix = capd::vectalg::Matrix<Interval, 0, 0>;

using Node = capd::autodiff::Node;

}
