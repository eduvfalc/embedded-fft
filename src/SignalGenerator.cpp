#include "SignalGenerator.hpp"
#include <math.h>
#include <numbers>
#include <vector>

void
SignalGenerator::GenerateSine(std::vector<std::pair<double, double>> parameters)
{
    for (std::chrono::nanoseconds dt(0); dt <= mDuration; dt += mSamplingPeriod) {
        double signal_value = 0;
        for (const auto& sine : parameters) {
            signal_value
                += std::sin(sine.first * std::numbers::pi * sine.second * std::chrono::duration<double>(dt).count());
        }
        Complex sample = {signal_value, 0};
        mSignal.emplace_back(sample);
    }
}