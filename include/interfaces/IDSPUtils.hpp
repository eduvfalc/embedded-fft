#ifndef H_IDSP_UTILS_HPP
#define H_IDSP_UTILS_HPP

#include <chrono>
#include <complex>
#include <vector>

using Complex = std::complex<double>;

class IDSPUtils
{
public:
    virtual ~IDSPUtils() = default;

    virtual void
    BitReversal(std::vector<Complex>& signal)
        = 0;

    virtual void
    ZeroPadding(std::vector<Complex>& signal)
        = 0;

    virtual std::vector<std::pair<double, double>>
    FindPeaks(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period, const int& max_peaks) = 0;
};

#endif  // H_DSP_UTILS_HPP