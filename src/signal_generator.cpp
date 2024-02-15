#include "signal_generator.hpp"
#include <math.h>
#include <numbers>
#include <vector>
#include "fft_types.hpp"

SignalGenerator::SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period)
  : mDuration(duration)
  , mSamplingPeriod(sampling_period){};

void
SignalGenerator::GenerateSines(std::vector<Complex>&                         signal,
                               const std::vector<std::pair<double, double>>& parameters) const
{
    for (std::chrono::nanoseconds dt(0); dt <= mDuration; dt += mSamplingPeriod) {
        double signal_value = 0;
        for (const auto& signal_params : parameters) {
            signal_value
                += signal_params.first
                   * std::sin(2 * std::numbers::pi * signal_params.second * std::chrono::duration<double>(dt).count());
        }
        Complex sample{signal_value, 0};
        signal.emplace_back(sample);
    }
}