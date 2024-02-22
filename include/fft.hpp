/**
 * @file fft.hpp
 * @author Eduardo Vieira Falcão
 * @brief Contains the definition of the FFT class
 */

#ifndef H_FFT_HPP
#define H_FFT_HPP

#include <cmath>
#include <complex>
#include <memory>
#include <numbers>
#include "dsp_utils.hpp"
#include "etl/vector.h"
#include "fft.hpp"
#include "fft_types.hpp"

namespace fftemb
{
/**
 * @brief Computes the in-place FFT transform
 *
 * @param[in,out] signal The signal to be transformed
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
void
compute(Container<T>& signal)
{
    if (!cnl::ispow2(signal.size())) {
        fft_utils::zero_padding(signal);
    }
    fft_utils::bit_reversal(signal);

    int n = signal.size();
    for (uint32_t len = 2; len <= n; len <<= 1) {
        auto angle = 2 * std::numbers::pi / len;
        T    wlen(std::cos(angle), std::sin(angle));

        for (uint32_t i = 0; i < n; i += len) {
            T w(1);
            for (uint32_t j = 0; j < len / 2; ++j) {
                auto u                  = signal[i + j];
                auto v                  = w * signal[i + j + len / 2];
                signal[i + j]           = u + v;
                signal[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

}  // namespace fftemb

#endif  // H_FFT_HPP