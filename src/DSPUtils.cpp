#include "DSPUtils.hpp"
#include <chrono>
#include <cmath>
#include <complex>
#include <vector>

using Complex = std::complex<double>;

void
DSPUtils::BitReversal(std::vector<Complex>& signal)
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
DSPUtils::ZeroPadding(std::vector<Complex>& signal)
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

std::pair<double, double>
DSPUtils::FindPeaks(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period)
{
    std::pair<double, double> peakData{0, 0};
    int                       signal_size = signal.size();
    for (int i = 1; i <= signal_size / 2; ++i) {
        auto amplitude = 2 * std::abs(signal[i]) / signal_size;
        if (amplitude > peakData.first) {
            peakData = {amplitude,
                        i * 1
                            / (signal_size
                               * std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count())};
        }
    }
    return peakData;
}
