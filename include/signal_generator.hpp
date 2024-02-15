#ifndef H_SIGNAL_GENERATOR_HPP
#define H_SIGNAL_GENERATOR_HPP

#include <chrono>
#include <complex>
#include <utility>
#include <vector>
#include "fft_types.hpp"

class SignalGenerator
{
public:
    SignalGenerator() = delete;

    SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period);

    void
    GenerateSines(std::vector<Complex>& signal, const std::vector<std::pair<double, double>>& parameters) const;

private:
    std::chrono::nanoseconds mDuration;
    std::chrono::nanoseconds mSamplingPeriod;
};

#endif  // H_SIGNAL_GENERATOR_HPP