/**
 * @file fft_types.hpp
 * @author Eduardo Vieira Falcão
 * @brief Defines the aliases for the FFT types
 */

#ifndef H_FFT_TYPES_HPP
#define H_FFT_TYPES_HPP

#include <complex>
#include "cnl/all.h"

namespace fftemb
{
/// @brief The type used to represent the values of the FFT calculations
using cnl_type
    = cnl::fixed_point<cnl::overflow_integer<cnl::rounding_integer<cnl::elastic_integer<31, int32_t>,
                                                                   cnl::_impl::tag_t<cnl::rounding_integer<>>>>,
                       -20>;

/// @brief The complex type
using Complex = std::complex<cnl_type>;

}  // namespace fftemb

#endif  // H_FFT_TYPES_HPP
