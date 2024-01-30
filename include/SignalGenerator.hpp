#ifndef H_SIGNAL_GENERATOR_HPP
#define H_SIGNAL_GENERATOR_HPP

#include <chrono>
#include <complex>
#include <utility>
#include <vector>

using Complex = std::complex<double>;

class SignalGenerator
{
public:
    SignalGenerator() = delete;
    SignalGenerator(std::vector<Complex>&    signal,
                    std::chrono::nanoseconds duration,
                    std::chrono::nanoseconds sampling_period)
      : mSignal(signal)
      , mDuration(duration)
      , mSamplingPeriod(sampling_period){};

    void
    GenerateSine(std::vector<std::pair<double, double>> parameters);

private:
    std::vector<Complex>&    mSignal;
    std::chrono::nanoseconds mDuration;
    std::chrono::nanoseconds mSamplingPeriod;
};

#endif  // H_SIGNAL_GENERATOR_HPP