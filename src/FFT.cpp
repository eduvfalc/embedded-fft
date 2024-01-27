#include "FFT.hpp"
#include <cmath>
#include <complex>

using Complex = std::complex<double>;

void FFT::Compute(std::vector<Complex>& signal, bool invert = false) {
    int n = signal.size();
    int levels = log2(n);

    // Bit-reversal permutation
    for (int i = 0; i < n; ++i) {
        int j = 0;
        for (int bit = 0; bit < levels; ++bit) {
            if (i & (1 << bit)) {
                j |= (1 << (levels - 1 - bit));
            }
        }
        if (j > i) {
            std::swap(signal[i], signal[j]);
        }
    }

    // Iterative FFT
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * M_PI / len * (invert ? -1 : 1);
        Complex wlen(cos(angle), sin(angle));

        for (int i = 0; i < n; i += len) {
            Complex w(1);
            for (int j = 0; j < len / 2; ++j) {
                Complex u = signal[i + j];
                Complex v = w * signal[i + j + len / 2];
                signal[i + j] = u + v;
                signal[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (int i = 0; i < n; ++i) {
            signal[i] /= n;
        }
    }
}