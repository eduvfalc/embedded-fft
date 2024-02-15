#include "signal_generator.hpp"
#include <math.h>
#include <numbers>
#include <vector>
#include "fft_types.hpp"

SignalGenerator::SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period)
  : m_duration(duration)
  , m_sampling_period(sampling_period){};

void
SignalGenerator::generate_sine_wave(std::vector<Complex>&                         signal,
                                    const std::vector<std::pair<double, double>>& parameters) const
{
    for (std::chrono::nanoseconds dt(0); dt <= m_duration; dt += m_sampling_period) {
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

void
SignalGenerator::generate_square_wave(std::vector<Complex>& signal, double frequency) const
{
    for (std::chrono::nanoseconds dt(0); dt <= m_duration; dt += m_sampling_period) {
        double signal_value = 0;
        signal_value
            += std::sin(2 * std::numbers::pi * frequency * std::chrono::duration<double>(dt).count()) > 0 ? 1 : -1;
        Complex sample{signal_value, 0};
        signal.emplace_back(sample);
    }
}