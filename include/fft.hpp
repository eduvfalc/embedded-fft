#ifndef H_FFT_HPP
#define H_FFT_HPP

#include <complex>
#include <memory>
#include <vector>
#include "dsp_utils.hpp"
#include "fft_types.hpp"

namespace fftemb
{
class FFT
{
public:
    FFT() = delete;
    explicit FFT(std::shared_ptr<DSPUtils> dsp_utils)
      : m_dsp_utils(dsp_utils){};

    void
    compute(std::vector<Complex>& signal);

private:
    std::shared_ptr<DSPUtils> m_dsp_utils = nullptr;
};
}  // namespace fftemb

#endif  // H_FFT_HPP