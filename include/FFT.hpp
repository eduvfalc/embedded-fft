#ifndef H_FFT_HPP
#define H_FFT_HPP

#include <complex>
#include <memory>
#include <vector>
#include "IDSPUtils.hpp"
#include "IFFT.hpp"

using Complex = std::complex<double>;

class FFT : public IFFT
{
public:
    FFT() = delete;
    explicit FFT(std::shared_ptr<IDSPUtils> fft_utils)
      : mDSPUtils(fft_utils){};

    void
    Compute(std::vector<Complex>& signal) override;

private:
    std::shared_ptr<IDSPUtils> mDSPUtils = nullptr;
};

#endif  // H_FFT_HPP