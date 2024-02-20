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
 * @brief Implements the in-place Cooley-Tukey's Fast Fourier Transform algorithm
 *
 * @tparam T The complex number type
 * @tparam Container The container type
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
class FFT
{
public:
    FFT() = delete;

    /**
     * @brief Construct a new FFT object
     *
     * @param dsp_utils The DSPUtils instance
     */
    explicit FFT(std::shared_ptr<DSPUtils<T, Container>> dsp_utils)
      : m_dsp_utils(dsp_utils){};

    /**
     * @brief Computes the FFT transform and stores the result in the
     *
     * @param[in,out] signal The signal to be transformed. Output overwrites signal.
     */
    void
    compute(Container<T>& signal);

private:
    /// @brief  The DSPUtils instance
    std::shared_ptr<DSPUtils<T, Container>> m_dsp_utils = nullptr;
};

template <typename T, template <class...> class Container>
void
FFT<T, Container>::compute(Container<T>& signal)
{
    m_dsp_utils->zero_padding(signal);
    m_dsp_utils->bit_reversal(signal);

    int n = signal.size();
    for (uint32_t len = 2; len <= n; len <<= 1) {
        double angle = 2 * std::numbers::pi / len;
        T      wlen(std::cos(angle), std::sin(angle));

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