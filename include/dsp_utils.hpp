#ifndef H_DSP_UTILS_HPP
#define H_DSP_UTILS_HPP

#include <chrono>
#include <complex>
#include <vector>

using Complex = std::complex<double>;

class DSPUtils
{
public:
    DSPUtils() = default;

    explicit DSPUtils(double max_percent_freq_delta)
      : mMaxPercentFrequencyDelta(max_percent_freq_delta){};

    void
    BitReversal(std::vector<Complex>& signal);

    void
    ZeroPadding(std::vector<Complex>& signal);

    std::vector<std::pair<double, double>>
    FindPeaks(std::vector<Complex>& signal, std::chrono::nanoseconds sampling_period, int max_peaks);

private:
    double mMaxPercentFrequencyDelta = 0.05;
};

#endif  // H_DSP_UTILS_HPP