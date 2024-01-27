#ifndef H_FFT_HPP
#define H_FFT_HPP

#include <complex>
#include <vector>
#include "IFFT.hpp"

using Complex = std::complex<double>;

class FFT : public IFFT {
    public:
        FFT() = default;
        void Compute(std::vector<Complex>& signal, bool invert) override;
};

#endif // H_FFT_HPP