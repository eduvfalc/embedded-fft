#ifndef H_SIGNAL_GENERATOR_HPP
#define H_SIGNAL_GENERATOR_HPP

#include <chrono>
#include <complex>
#include <utility>
#include <vector>
#include "etl/vector.h"
#include "fft_types.hpp"

namespace fftemb
{
class SignalGenerator
{
public:
    SignalGenerator() = delete;

    SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period);

    void
    generate_sine_wave(etl::ivector<Complex>& signal, const std::vector<std::pair<double, double>>& parameters) const;

    void
    generate_square_wave(etl::ivector<Complex>& signal, double frequency) const;

private:
    std::chrono::nanoseconds m_duration;
    std::chrono::nanoseconds m_sampling_period;
};
}  // namespace fftemb

#endif  // H_SIGNAL_GENERATOR_HPP