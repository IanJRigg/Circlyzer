#include "circuit_analyzer/phasors.h"

using namespace std::complex_literals;

/**********************************************************************************************//**
 * \brief 
 * \param elements 
 *************************************************************************************************/
std::complex<double>
simplify_series(const std::vector<std::complex<double>>& elements)
{
	auto result = 0.0 + 0i;

    for(const auto& element : elements)
    {
    	result += element;
    }

    return result;
}

/**********************************************************************************************//**
 * \brief 
 * \param argc elements 
 *************************************************************************************************/
std::complex<double>
simplify_parallel(const std::vector<std::complex<double>>& elements)
{
    auto accumulator = 0.0 + 0i;

    for(const auto& element : elements)
    {
       accumulator += (1.0 / element);
    }

    return (1.0 / accumulator);
}

/**********************************************************************************************//**
 * \brief 
 * \param one
 * \param two 
 *************************************************************************************************/
std::complex<double>
simplify_parallel(const std::complex<double>& one, const std::complex<double>& two)
{
    return (one * two) / (one + two);
}

/**********************************************************************************************//**
 * \brief 
 * \param elements 
 *************************************************************************************************/
std::complex<float>
simplify_series(const std::vector<std::complex<float>>& elements)
{
    auto result = 0.0f + 0if;

    for(const auto& element : elements)
    {
    	result += element;
    }

    return result;
}

/**********************************************************************************************//**
 * \brief 
 * \param elements 
 *************************************************************************************************/
std::complex<float>
simplify_parallel(const std::vector<std::complex<float>>& elements)
{
    auto accumulator = 0.0f + 0if;

    for(const auto& element : elements)
    {
       accumulator += (1.0f / element);
    }

    return (1.0f / accumulator);
}

/**********************************************************************************************//**
 * \brief 
 * \param one
 * \param two 
 *************************************************************************************************/
std::complex<float>
simplify_parallel(const std::complex<float>& one, const std::complex<float>& two)
{
    return (one * two) / (one + two);
}