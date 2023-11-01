///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/ode_solver.hpp"
#include "capd/timemap.hpp"
#include "capd/solution_curve.hpp"

#include "capd/c0rect2set.hpp"
#include "capd/c1rect2set.hpp"

#include "map_base.hpp"

namespace Carina
{

template<typename MapT, bool is_interval>
class TimemapWrapperInternal
{};

template<typename MapT>
class TimemapWrapperInternal<MapT, false>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	TimemapWrapperInternal(Timemap<MapT>& timemap, const ScalarType& time, unsigned dimension)
		: m_timemap(timemap)
		, m_time(time)
		, m_vec(dimension)
		, m_mat(dimension, dimension)
	{}

	VectorType operator() (const VectorType& vec)
	{
		m_vec = vec;
		return m_timemap(m_time, m_vec);
	}

    VectorType operator() (const VectorType& vec, MatrixType& der)
	{
		m_vec = vec;
		return m_timemap(m_time, m_vec, der);
	}

	void operator() (const VectorType& vec, SolutionCurve<MapT>& solution_curve)
	{
		m_vec = vec;
		m_timemap(m_time, m_vec, m_mat, solution_curve);
	}

private:
	Timemap<MapT>& m_timemap;
	const ScalarType& m_time;

	VectorType m_vec;
	MatrixType m_mat;
};

template<typename MapT>
class TimemapWrapperInternal<MapT, true>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	TimemapWrapperInternal(Timemap<MapT>& timemap, const ScalarType& time, unsigned)
		: m_timemap(timemap)
		, m_time(time)
	{}

	VectorType operator() (const VectorType& vec)
	{
		C0Rect2Set<MapT> set(vec);
		return m_timemap(m_time, set);
	}

    VectorType operator() (const VectorType& vec, MatrixType& der)
	{
		C1Rect2Set<MapT> set(vec);
		return m_timemap(m_time, set, der);
	}

	void operator() (const VectorType& vec, SolutionCurve<MapT>& solution_curve)
	{
		C1Rect2Set<MapT> set(vec);
		m_timemap(m_time, set, solution_curve);
	}

private:
	Timemap<MapT>& m_timemap;
	const ScalarType& m_time;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Timemap implementation
//!
//! Calculation of a flow of a given vector field of a fixed time.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class TimemapWrapper : public MapBase<MapT>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @brief Constructor
	//!
	//! @param vector_field vector field to be integrated
	//! @param time         integration_time
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TimemapWrapper(MapT& vector_field, ScalarType time = ScalarType(0.0), unsigned order = 20)
		: m_vector_field(vector_field)
		, m_time(time)
		, m_solver(vector_field, order)
		, m_timemap(m_solver)
		, m_timemap_internal(m_timemap, m_time, m_vector_field.dimension())
	{}

	VectorType operator() (const VectorType& vec) override
	{
		return m_timemap_internal(vec);
	}

    VectorType operator() (const VectorType& vec, MatrixType& der) override
	{
		return m_timemap_internal(vec, der);
	}

	void operator() (const VectorType& vec, SolutionCurve<MapT>& solution_curve)
	{
		m_timemap_internal(vec, solution_curve);
	}

	unsigned dimension() const override
	{
		return m_vector_field.dimension();
	}

	unsigned imageDimension() const override
	{
		return m_vector_field.imageDimension();
	}

	void set_time(ScalarType time)
	{
		m_time = time;
	}

	ScalarType get_time() const noexcept
	{
		return m_time;
	}

private:
	MapT& m_vector_field;
	ScalarType m_time;

	OdeSolver<MapT> m_solver;
	Timemap<MapT> m_timemap;

	static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
	TimemapWrapperInternal<MapT, is_interval> m_timemap_internal;
};

}
