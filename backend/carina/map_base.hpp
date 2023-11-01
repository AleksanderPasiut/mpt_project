///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>
#include <stdexcept>
#include <string>
#include <sstream>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Base class for custom maps
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class MapBase
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    virtual VectorType operator() (const VectorType&) = 0;

    virtual VectorType operator() (const VectorType&, MatrixType&) = 0;

    virtual unsigned dimension() const = 0;

    virtual unsigned imageDimension() const = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Utility function allowing to assert the size of matrix
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void assert_matrix_size(
        const MatrixType& matrix,
        unsigned rows,
        unsigned cols,
        const std::string& message)
    {
        const std::pair<unsigned, unsigned> dim = matrix.dimension();
        if (dim != std::make_pair(rows, cols))
        {
            std::stringstream ss;
            ss << message << '\n';
            ss << "Expected: (" << rows << ", " << cols << ")\n";
            ss << "Actual: (" << dim.first << ", " << dim.second << ")";
            throw std::logic_error(ss.str());
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Utility function allowing to assert the size of vector
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void assert_vector_size(
        const VectorType& vector,
        unsigned dimension,
        const std::string& message)
    {
        if (dimension != vector.dimension())
        {
            std::stringstream ss;
            ss << message << '\n';
            ss << "Expected: " << dimension << "\n";
            ss << "Actual: " << vector.dimension();
            throw std::logic_error(ss.str());
        }
    }
};

}
