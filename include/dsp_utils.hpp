/**
 * @file dsp_utils.hpp
 * @author Eduardo Vieira Falcão
 * @brief Contains the definition of the template class DSPUtils
 */

#ifndef H_DSP_UTILS_HPP
#define H_DSP_UTILS_HPP

#include <chrono>
#include <cmath>
#include <complex>
#include <numbers>
#include <vector>
#include "etl/vector.h"
#include "fft_types.hpp"

namespace fftemb::fft_utils
{
/**
 * @brief Apply the bit reversal permutation to the container
 *
 * @param[in,out] sequence The sequence of elements
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
void
bit_reversal(Container<T>& sequence)
{
    auto sequence_size = sequence.size();
    auto levels        = cnl::log2p1(sequence_size) - 1;

    for (int i = 0; i < sequence_size; ++i) {
        int j = 0;
        for (int bit = 0; bit < levels; ++bit) {
            if (i & (1 << bit)) {
                j |= (1 << (levels - 1 - bit));
            }
        }
        if (j > i) {
            std::swap(sequence[i], sequence[j]);
        }
    }
}


/**
 * @brief Append zeros to the sequence until the next power of 2, if applicable
 *
 * @param[in,out] sequence The sequence of elements
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
void
zero_padding(Container<T>& sequence)
{
    int sequence_size = sequence.size();
    int nextPow2      = 1;
    while (nextPow2 < sequence_size) {
        nextPow2 <<= 1;
    }

    if (nextPow2 > sequence_size) {
        sequence.resize(nextPow2, {0, 0});
    }
}

/**
 * @brief Normalize a sequence of complex numbers
 *
 * @param[in,out] sequence The sequence of elements
 * @return The longest norm of the sequence
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
auto
normalize(Container<T>& sequence)
{
    auto max_it        = std::max_element(sequence.begin(), sequence.end(), [](const T& a, const T& b) {
        return abs(a) < abs(b);
    });
    auto max_amplitude = cnl::sqrt(max_it->real() * max_it->real() + max_it->imag() * max_it->imag());
    std::transform(sequence.begin(), sequence.end(), sequence.begin(), [max_amplitude](auto& bin) {
        return T(cnl::quotient(bin.real(), max_amplitude), cnl::quotient(bin.imag(), max_amplitude));
    });
    return max_amplitude;
}

/**
 * @brief Sequence to mulitply by the Hann window
 *
 * @param[in,out] sequence The sequence of elements
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
void
apply_hann_window(Container<T>& sequence)
{
    T    correction_fator{2, 0};
    auto sequence_size = sequence.size();
    for (int i = 0; i <= sequence_size; ++i) {
        T hanning_bin = {0.5 - 0.5 * std::cos(2 * std::numbers::pi * i / sequence_size), 0};
        sequence[i]   = sequence[i] * hanning_bin * correction_fator;
    }
}

/**
 * @brief Calculates the norm of a complex number using CNL's sqrt
 *
 * @param complex The complex number
 * @return The norm
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
auto
abs(T complex)
{
    return cnl::sqrt(complex.real() * complex.real() + complex.imag() * complex.imag());
}

}  // namespace fftemb::fft_utils

#endif  // H_DSP_UTILS_HPP
