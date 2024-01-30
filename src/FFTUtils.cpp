#include "FFTUtils.hpp"
#include <cmath>
#include <complex>
#include <vector>

using Complex = std::complex<double>;

void
FFTUtils::BitReversal(std::vector<Complex>& signal)
{
    auto n      = signal.size();
    int  levels = log2(n);

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
}

void
FFTUtils::ZeroPadding(std::vector<Complex>& signal)
{
    int signalSize = signal.size();
    int nextPow2   = 1;
    while (nextPow2 < signalSize) {
        nextPow2 <<= 1;
    }

    if (nextPow2 > signalSize) {
        signal.resize(nextPow2, 0.0);
    }
}