#ifndef H_SIGNAL_GENERATOR_HPP
#define H_SIGNAL_GENERATOR_HPP

#include <math.h>
#include <numbers>
#include <vector>
#include "etl/vector.h"
#include "fft_types.hpp"

namespace fftemb
{
template <typename T = Complex, template <class...> class Container = etl::ivector>
class SignalGenerator
{
public:
    SignalGenerator() = default;

    SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period)
      : m_duration(duration)
      , m_sampling_period(sampling_period){};

    void
    generate_sine_wave(Container<T>& signal, const std::vector<std::pair<double, double>>& parameters) const;

    void
    generate_square_wave(Container<T>& signal, double frequency) const;

private:
    std::chrono::nanoseconds m_duration;
    std::chrono::nanoseconds m_sampling_period;
};

template <typename T, template <class...> class Container>
void
SignalGenerator<T, Container>::generate_sine_wave(Container<T>&                                 signal,
                                                  const std::vector<std::pair<double, double>>& parameters) const
{
    int i = 0;
    for (std ::chrono::nanoseconds dt(0); dt <= m_duration; dt += m_sampling_period, ++i) {
        double signal_value = 0;
        for (const auto& signal_params : parameters) {
            signal_value
                += signal_params.first
                   * std::sin(2 * std::numbers::pi * signal_params.second * std::chrono::duration<double>(dt).count());
        }
        signal[i] = T{signal_value, 0};
    }
}

template <typename T, template <class...> class Container>
void
SignalGenerator<T, Container>::generate_square_wave(Container<T>& signal, double frequency) const
{
    int i = 0;
    for (std::chrono::nanoseconds dt(0); dt <= m_duration; dt += m_sampling_period, ++i) {
        double signal_value = 0;
        signal_value
            = std::sin(2 * std::numbers::pi * frequency * std::chrono::duration<double>(dt).count()) >= 0 ? 1 : -1;
        signal[i] = T{signal_value, 0};
    }
}

}  // namespace fftemb

#endif  // H_SIGNAL_GENERATOR_HPP