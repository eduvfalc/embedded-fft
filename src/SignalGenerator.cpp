#include "SignalGenerator.hpp"
#include <math.h>
#include <vector>

const double PI = 2 * std::acos(0.0);

void
SignalGenerator::GenerateSine(std::vector<std::pair<double, double>> parameters)
{
    for (std::chrono::nanoseconds dt(0); dt <= mDuration; dt += mSamplingPeriod) {
        double signal_value = 0;
        for (const auto& sine : parameters) {
            signal_value += std::sin(sine.first * PI * sine.second * std::chrono::duration<double>(dt).count());
        }
        Complex sample = {signal_value, 0};
        mSignal.emplace_back(sample);
    }
}