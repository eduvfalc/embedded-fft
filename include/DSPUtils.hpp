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
};

#endif  // H_DSP_UTILS_HPP