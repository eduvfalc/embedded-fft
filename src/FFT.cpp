#include "FFT.hpp"
#include <cmath>
#include <complex>
#include <memory>
#include <numbers>
#include <vector>

using Complex = std::complex<double>;

void
FFT::Compute(std::vector<Complex>& signal)
{
    mFFTUtils->ZeroPadding(signal);
    mFFTUtils->BitReversal(signal);

    int n = signal.size();
    for (uint32_t len = 2; len <= n; len <<= 1) {
        double  angle = 2 * std::numbers::pi / len;
        Complex wlen(std::cos(angle), std::sin(angle));

        for (uint32_t i = 0; i < n; i += len) {
            Complex w(1);
            for (uint32_t j = 0; j < len / 2; ++j) {
                Complex u               = signal[i + j];
                Complex v               = w * signal[i + j + len / 2];
                signal[i + j]           = u + v;
                signal[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}