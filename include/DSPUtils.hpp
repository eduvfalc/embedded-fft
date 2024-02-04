#ifndef H_DSP_UTILS_HPP
#define H_DSP_UTILS_HPP

#include <complex>
#include <vector>
#include "IDSPUtils.hpp"

using Complex = std::complex<double>;

class DSPUtils : public IDSPUtils
{
public:
    DSPUtils() = default;

    void
    BitReversal(std::vector<Complex>& signal) override;

    void
    ZeroPadding(std::vector<Complex>& signal) override;

    std::vector<std::pair<double, double>>
    FindPeaks(std::vector<Complex>&           signal,
              const std::chrono::nanoseconds& sampling_period,
              const int&                      max_peaks) override;
};

#endif  // H_DSP_UTILS_HPP