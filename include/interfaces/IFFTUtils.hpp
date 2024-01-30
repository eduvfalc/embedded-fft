#ifndef H_IFFT_UTILS_HPP
#define H_IFFT_UTILS_HPP

#include <complex>
#include <vector>

using Complex = std::complex<double>;

class IFFTUtils
{
public:
    virtual ~IFFTUtils() = default;
    virtual void BitReversal(std::vector<Complex>& signal) = 0;
    virtual void ZeroPadding(std::vector<Complex>& signal) = 0;
};

#endif // H_IFFT_UTILS_HPP