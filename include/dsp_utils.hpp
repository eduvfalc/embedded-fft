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

namespace fftemb
{
/**
 * @brief Helper class for digital signal processing subtasks
 *
 * @tparam T The complex number type
 * @tparam Container The containe type
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
class DSPUtils
{
public:
    /**
     * @brief Construct a new DSPUtils object
     *
     */
    DSPUtils() = default;

    /**
     * @brief Construct a new DSPUtils object
     *
     * @param min_frequency_delta_pct Minimum percentual frequency difference used to qualify two adjacent peaks as a
     * different peak
     * @param max_dc_leakage_frequency_hz Maximum frequency value that defines a DC component and its spectral leakage
     */
    DSPUtils(double min_frequency_delta_pct, double max_dc_leakage_frequency_hz)
      : m_min_frequency_delta_pct(min_frequency_delta_pct)
      , m_max_dc_leakage_frequency_hz(max_dc_leakage_frequency_hz){};

    /**
     * @brief Apply the bit reversal permutation to the container
     *
     * @param[in,out] sequence The sequence of elements
     */
    void
    bit_reversal(Container<T>& sequence) const;

    /**
     * @brief Append zeros to the sequence until the next power of 2, if applicable
     *
     * @param[in,out] sequence The sequence of elements
     */
    void
    zero_padding(Container<T>& sequence) const;

    /**
     * @brief Normalize a sequence of complex numbers
     *
     * @param[in,out] sequence The sequence of elements
     * @return The longest norm of the sequence
     */
    auto
    normalize(Container<T>& sequence) const;

    /**
     * @brief Sequence to mulitply by the Hann window
     *
     * @param[in,out] sequence The sequence of elements
     */
    void
    apply_hann_window(Container<T>& sequence) const;

    /**
     * @brief Calculates the norm of a complex number using CNL's sqrt
     *
     * @param complex The complex number
     * @return The norm
     */
    auto
    abs(T complex) const;

private:
    /// @brief Minimum percentual frequency difference used to qualify two adjacent peaks as a different peak
    double m_min_frequency_delta_pct = 0.05;
    /// @brief Maximum frequency value that defines a DC component and its spectral leakage
    double m_max_dc_leakage_frequency_hz = 2;
};

template <typename T, template <class...> class Container>
void
DSPUtils<T, Container>::bit_reversal(Container<T>& sequence) const
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

template <typename T, template <class...> class Container>
void
DSPUtils<T, Container>::zero_padding(Container<T>& sequence) const
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

template <typename T, template <class...> class Container>
auto
DSPUtils<T, Container>::normalize(Container<T>& sequence) const
{
    auto max_it        = std::max_element(sequence.begin(), sequence.end(), [this](const T& a, const T& b) {
        return abs(a) < abs(b);
    });
    auto max_amplitude = cnl::sqrt(max_it->real() * max_it->real() + max_it->imag() * max_it->imag());
    std::transform(sequence.begin(), sequence.end(), sequence.begin(), [max_amplitude](auto& bin) {
        return T(cnl::quotient(bin.real(), max_amplitude), cnl::quotient(bin.imag(), max_amplitude));
    });
    return max_amplitude;
}

template <typename T, template <class...> class Container>
void
DSPUtils<T, Container>::apply_hann_window(Container<T>& sequence) const
{
    T    correction_fator{2, 0};
    auto sequence_size = sequence.size();
    for (int i = 0; i <= sequence_size; ++i) {
        T hanning_bin = {0.5 - 0.5 * std::cos(2 * std::numbers::pi * i / sequence_size), 0};
        sequence[i]   = sequence[i] * hanning_bin * correction_fator;
    }
}

template <typename T, template <class...> class Container>
auto
DSPUtils<T, Container>::abs(T complex) const
{
    return cnl::sqrt(complex.real() * complex.real() + complex.imag() * complex.imag());
}

}  // namespace fftemb

#endif  // H_DSP_UTILS_HPP
