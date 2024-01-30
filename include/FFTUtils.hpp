#ifndef H_FFT_UTILS_HPP
#define H_FFT_UTILS_HPP

#include "IFFTUtils.hpp"
#include <complex>
#include <vector>

using Complex = std::complex<double>;

class FFTUtils : public IFFTUtils
{
public:
    FFTUtils() = default;
    void BitReversal(std::vector<Complex>& signal) override;
    void ZeroPadding(std::vector<Complex>& signal) override;
};

#endif // H_FFT_UTILS_HPP