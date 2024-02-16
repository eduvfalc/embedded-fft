#include "fft.hpp"
#include <cmath>
#include <complex>
#include <memory>
#include <numbers>
#include "etl/vector.h"
#include "fft_types.hpp"

using namespace fftemb;

void
FFT::compute(etl::ivector<Complex>& signal)
{
    m_dsp_utils->zero_padding(signal);
    m_dsp_utils->bit_reversal(signal);

    int n = signal.size();
    for (uint32_t len = 2; len <= n; len <<= 1) {
        double  angle = 2 * std::numbers::pi / len;
        Complex wlen(std::cos(angle), std::sin(angle));

        for (uint32_t i = 0; i < n; i += len) {
            Complex w(1);
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