#ifndef H_FFT_HPP
#define H_FFT_HPP

#include <vector>
#include <complex>
#include "IFFT.hpp"
#include "IFFTUtils.hpp"

using Complex = std::complex<double>;

class FFT : public IFFT {
    public:
        FFT() = delete;
        explicit FFT(std::shared_ptr<IFFTUtils> fft_utils) : mFFTUtils(fft_utils) {};
        void Compute(std::vector<Complex>& signal) override;

    private:
        std::shared_ptr<IFFTUtils> mFFTUtils = nullptr;
};

#endif // H_FFT_HPP