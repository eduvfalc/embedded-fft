#include "SignalGenerator.hpp"
#include <math.h>
#include <numbers>
#include <vector>

SignalGenerator::SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period)
  : mDuration(duration)
  , mSamplingPeriod(sampling_period){};

void
SignalGenerator::GenerateSine(std::vector<Complex>& signal, std::pair<double, double>& parameters)
{
    for (std::chrono::nanoseconds dt(0); dt <= mDuration; dt += mSamplingPeriod) {
        double signal_value = 0;
        signal_value        = parameters.first
                       * std::sin(2 * std::numbers::pi * parameters.second * std::chrono::duration<double>(dt).count());
        Complex sample = {signal_value, 0};
        signal.emplace_back(sample);
    }
}