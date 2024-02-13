#ifndef H_FFT_HPP
#define H_FFT_HPP

#include <complex>
#include <memory>
#include <vector>
#include "dsp_utils.hpp"

using Complex = std::complex<double>;

class FFT
{
public:
    FFT() = delete;
    explicit FFT(std::shared_ptr<DSPUtils> dsp_utils)
      : mDSPUtils(dsp_utils){};

    void
    Compute(std::vector<Complex>& signal);

private:
    std::shared_ptr<DSPUtils> mDSPUtils = nullptr;
};

#endif  // H_FFT_HPP